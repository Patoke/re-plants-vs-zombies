#include "VkImage.h"
#include "Common.h"
#include "VkCommon.h"
#include <chrono>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <array>

namespace Vk {

VkImage::VkImage(ImageLib::Image &theImage)
{
    renderMutex.lock();

    mWidth = theImage.mWidth;
    mHeight = theImage.mHeight;

    VkDeviceSize imageSize = mWidth * mHeight * sizeof(uint32_t);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    
    createBuffer(imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, theImage.mBits.get(), imageSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createImage(mWidth, mHeight,
        VK_FORMAT_B8G8R8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        image,
        memory
    );

    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        endSingleTimeCommands(commandBuffer);
    }

    copyBufferToImage(stagingBuffer, image, mWidth, mHeight);

    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        endSingleTimeCommands(commandBuffer);
    }
    

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    view = createImageView(image, VK_FORMAT_B8G8R8A8_SRGB);

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = imagePass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &view;
    framebufferInfo.width = mWidth;
    framebufferInfo.height = mHeight;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }

    renderMutex.unlock();
}

VkImage::~VkImage() {
    renderMutex.lock();
    flushCommandBuffer();
    cachedOtherImage = nullptr; // Force cache miss.
    vkDeviceWaitIdle(device);
    vkDestroyImageView(device, view, nullptr);
    vkDestroyImage(device, image, nullptr);
    vkFreeMemory(device, memory, nullptr);
    renderMutex.unlock();
}

void VkImage::TransitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout) {
    //if (layout == newLayout) return;
    transitionImageLayout(commandBuffer, image, layout, newLayout);
    layout = newLayout;
}

/*================*
 | DRAW FUNCTIONS |
 *================*/

void VkImage::FillRect(const Rect& /*theRect*/, const Color& /*theColor*/, int /*theDrawMode*/){
    /*renderMutex.lock();
    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    auto color = VkClearColorValue{ .float32 = {theColor.mRed/255.0f, theColor.mGreen/255.0f, theColor.mBlue/255.0f} };

    auto subresourceRange = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdClearColorImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &subresourceRange);

    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    endSingleTimeCommands(commandBuffer);
    
    renderMutex.unlock();*/
}

VkImage* cachedOtherImage = nullptr;
bool inRecording = false;
bool inRenderpass = false;
int imageBufferIdx = 0;
constexpr float display_interval_sec = 4;
int num_flushes = 0;
int num_blits = 0;
int num_passes = 0;
auto begin_time = std::chrono::high_resolution_clock::now();
void flushCommandBuffer() {
    if (inRecording) {
        num_flushes++;
        auto now_time = std::chrono::high_resolution_clock::now();
        auto time_passed = std::chrono::duration_cast<std::chrono::duration<double>>(
            now_time - begin_time).count();
        if (time_passed > display_interval_sec) {
            printf("\nblit/flush: %f\npass/frame: %f\nblts/frame: %f\n framerate: %f\n",
                (double)num_flushes/num_blits,
                num_passes/(gFramerate*time_passed),
                num_blits/(gFramerate*time_passed),
                gFramerate
            );
            begin_time = now_time;

            num_flushes = 0;
            num_blits = 0;
            num_passes = 0;
        }
        if(inRenderpass) {
            num_passes++;
            vkCmdEndRenderPass(imageCommandBuffers[imageBufferIdx]);
            inRenderpass = false;
        }

        vkEndCommandBuffer(imageCommandBuffers[imageBufferIdx]);
        inRecording = false;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &imageCommandBuffers[imageBufferIdx];

        vkWaitForFences(device, 1, &imageFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &imageFence);
        vkQueueSubmit(graphicsQueue, 1, &submitInfo, imageFence);
        //vkQueueWaitIdle(graphicsQueue);
        imageBufferIdx = (imageBufferIdx + 1) % num_image_swaps;
        
        vkResetCommandBuffer(imageCommandBuffers[imageBufferIdx], 0);
    }
}

void VkImage::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color&/*theColor*/, int/*theDrawMode*/){
    renderMutex.lock();

    num_blits++;

    VkImage *otherImage = dynamic_cast<VkImage*>(theImage);
    bool cacheMiss = (otherImage != cachedOtherImage);
    cachedOtherImage = otherImage;

    bool thisLayoutSuboptimal =  (layout != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    bool otherLayoutSuboptimal = (otherImage->layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    if (cacheMiss) flushCommandBuffer(); // Have to rebind descriptor set, forces a flush.
    if (!inRecording) { // Start recording the command buffer
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(imageCommandBuffers[imageBufferIdx], &beginInfo);
        inRecording = true;
    }
    if (cacheMiss) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = otherImage->view;
        imageInfo.sampler = textureSampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = imageDescriptors[imageBufferIdx];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            device,
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(),
            0,
            nullptr
        );
        vkCmdBindDescriptorSets(imageCommandBuffers[imageBufferIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &imageDescriptors[imageBufferIdx], 0, nullptr);
        vkCmdBindPipeline(imageCommandBuffers[imageBufferIdx], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }
    if (thisLayoutSuboptimal || otherLayoutSuboptimal) {
        std::array<VkImageMemoryBarrier, 2> barriers{};
        if (thisLayoutSuboptimal) {
            barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barriers[0].oldLayout = layout;
            barriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            barriers[0].image = image;
            barriers[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barriers[0].subresourceRange.baseMipLevel = 0;
            barriers[0].subresourceRange.levelCount = 1;
            barriers[0].subresourceRange.baseArrayLayer = 0;
            barriers[0].subresourceRange.layerCount = 1;
            barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        if (otherLayoutSuboptimal) {
            barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barriers[1].oldLayout = otherImage->layout;
            barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barriers[1].image = otherImage->image;
            barriers[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barriers[1].subresourceRange.baseMipLevel = 0;
            barriers[1].subresourceRange.levelCount = 1;
            barriers[1].subresourceRange.baseArrayLayer = 0;
            barriers[1].subresourceRange.layerCount = 1;
            barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            otherImage->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        vkCmdPipelineBarrier(
            imageCommandBuffers[imageBufferIdx],
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            (thisLayoutSuboptimal && otherLayoutSuboptimal) ? 2 : 1, &barriers[otherLayoutSuboptimal && (!thisLayoutSuboptimal)]
        );
    }

    if (!inRenderpass) {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = imagePass;
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {static_cast<uint32_t>(mWidth), static_cast<uint32_t>(mHeight)};
        vkCmdBeginRenderPass(imageCommandBuffers[imageBufferIdx], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        inRenderpass = true;
    }

    VkViewport viewport{};
    viewport.x = theX - theSrcRect.mX;
    viewport.y = theY - theSrcRect.mY;
    viewport.width = static_cast<float>(theImage->mWidth);
    viewport.height = static_cast<float>(theImage->mHeight);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(imageCommandBuffers[imageBufferIdx], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {theX, theY};
    scissor.extent = {static_cast<uint32_t>(theSrcRect.mWidth), static_cast<uint32_t>(theSrcRect.mHeight)};
    vkCmdSetScissor(imageCommandBuffers[imageBufferIdx], 0, 1, &scissor);

    vkCmdDraw(imageCommandBuffers[imageBufferIdx], 3,1,0,0);

    renderMutex.unlock();
}

void VkImage::BltF(Image*/*theImage*/, float /*theX*/, float /*theY*/, const Rect& /*theSrcRect*/, const Rect &/*theClipRect*/, const Color& /*theColor*/, int /*theDrawMode*/){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltF is a stub.\n");
    has_shown = true;
}

void VkImage::BltMatrix(Image* /*theImage*/, float /*x*/, float /*y*/, const SexyMatrix3 &/*theMatrix*/, const Rect& /*theClipRect*/, const Color& /*theColor*/, int /*theDrawMode*/, const Rect &/*theSrcRect*/, bool /*blend*/){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltMatrix is a stub.\n");
    has_shown = true;
}

/*=================*
 |  UNIMPLEMENTED  |
 *=================*/

void VkImage::ClearRect(const Rect& /*theRect*/){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::ClearRect is a stub.\n");
    has_shown = true;
}

void VkImage::DrawLine(double, double, double, double, const Color&, int){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::DrawLine is a stub.\n");
    has_shown = true;
}

void VkImage::DrawLineAA(double, double, double, double, const Color&, int){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::DrawLineAA is a stub.\n");
    has_shown = true;
}

void VkImage::FillScanLinesWithCoverage(Span*, int, const Color&, int, const BYTE*, int, int, int, int){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::FillScanLinesWithCoverage is a stub.\n");
    has_shown = true;
}

bool VkImage::PolyFill3D(const Point*, int, const Rect*, const Color&,int ,int, int)
{
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::PolyFill3D is a stub.\n");
    has_shown = true;

    return false;
}

void VkImage::BltRotated(Image*, float, float, const Rect &, const Rect&, const Color&, int, double, float, float){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltRotated is a stub.\n");
    has_shown = true;
}

void VkImage::StretchBlt(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::StretchBlt is a stub.\n");
    has_shown = true;
}

void VkImage::BltTrianglesTex(Image*, const TriVertex (*)[3], int, const Rect&, const Color &, int, float, float, bool){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltTrianglesTex is a stub.\n");
    has_shown = true;
}

void VkImage::BltMirror(Image*, int, int, const Rect&, const Color&, int){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltMirror is a stub.\n");
    has_shown = true;
}

void VkImage::StretchBltMirror(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::StretchBltMirror is a stub.\n");
    has_shown = true;
}

} // namespace Vk

/*
void Image::FillScanLinesWithCoverage(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode, const BYTE* theCoverage, int theCoverX, int theCoverY, int theCoverWidth, int theCoverHeight);
bool Image::PolyFill3D(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty, bool convex);
void Image::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode);
void Image::BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect &theClipRect, const Color& theColor, int theDrawMode);
void Image::BltRotated(Image* theImage, float theX, float theY, const Rect &theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY);
void Image::StretchBlt(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch);
void Image::BltMatrix(Image* theImage, float x, float y, const SexyMatrix3 &theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, bool blend);
void Image::BltTrianglesTex(Image *theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color &theColor, int theDrawMode, float tx, float ty, bool blend);
void Image::BltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode);
void Image::StretchBltMirror(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch);
*/
