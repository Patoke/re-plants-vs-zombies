#include "VkImage.h"

#include "Common.h"
#include "VkCommon.h"

#include "TriVertex.h"
#include "misc/SexyMatrix.h"

#include <glm/gtx/matrix_transform_2d.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <array>
#include <vulkan/vulkan_core.h>

namespace Vk {

void createImage(int width, int height, ::VkImage &image, VkDeviceMemory &memory, VkImageUsageFlags usage) {
    static VkImageCreateInfo imageInfo {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        pixelFormat,
        {0, 0, 1}, // to be filled in later
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        0, // to be filled in later
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr,
        VK_IMAGE_LAYOUT_UNDEFINED
    };

    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.usage = usage;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, memory, 0);
}

void VkImage::UpdateDescriptorSets() {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = view;
    imageInfo.sampler = textureSampler;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptor;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(
        device,
        1,
        &descriptorWrite,
        0,
        nullptr
    );
}

VkFramebuffer VkImage::CreateFramebuffer() {
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = imagePass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &view;
    framebufferInfo.width = mWidth;
    framebufferInfo.height = mHeight;
    framebufferInfo.layers = 1;

    VkFramebuffer fb;
    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &fb) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }

    return fb;
}

int descriptorPoolSize = 0;
VkImage::VkImage(ImageLib::Image &theImage)
{
    mWidth = theImage.mWidth;
    mHeight = theImage.mHeight;

    if (!mWidth || !mHeight) throw std::runtime_error("Images with no size are not supported.");

    renderMutex.lock();

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

    createImage(
        mWidth, mHeight,
        image, memory,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT
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

    view = createImageView(image, pixelFormat);

    framebuffer = CreateFramebuffer();

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    vkAllocateDescriptorSets(device, &allocInfo, &descriptor);
    ++descriptorPoolSize;

    UpdateDescriptorSets();

    renderMutex.unlock();
}

struct deleteInfo {
    std::optional<::VkImage> image;
    std::optional<VkImageView> view;
    std::optional<VkFramebuffer> framebuffer;
    std::optional<VkDeviceMemory> memory;
    std::optional<VkDescriptorSet> set;
};

int imageBufferIdx = 0;
std::array<std::vector<deleteInfo>, NUM_IMAGE_SWAPS> deleteList;
VkImage::~VkImage() {
    //renderMutex.lock();
    //flushCommandBuffer();
    //vkDeviceWaitIdle(device);

    deleteList[imageBufferIdx].emplace_back(deleteInfo{
        image, view, framebuffer, memory, descriptor
    });

    /*
    */
    //renderMutex.unlock();
}

/*====================*
 | GRAPHICS FUNCTIONS |
 *====================*/

template <decltype(auto) arr>
static constexpr auto const_generate_layout_map() {
    using T = decltype(arr)::value_type::second_type;

    constexpr auto max_key = std::max_element(arr.begin(), arr.end(),
        [](const auto &left, const auto &right) {
            return left.first < right.first;
        })->first;

    std::array<T, max_key + 1> sparse_array{};

    for (auto it : arr) {
        sparse_array[it.first] = it.second;
    }

    return sparse_array;
}

constexpr auto accessMaskMap = const_generate_layout_map<std::array<std::pair<VkImageLayout, std::pair<VkAccessFlags, VkPipelineStageFlags>>, 5> {{
    {VK_IMAGE_LAYOUT_UNDEFINED,                {0,                                                                          VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}},
    {VK_IMAGE_LAYOUT_GENERAL,                  {VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT,                     VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}},
    {VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,     {VK_ACCESS_TRANSFER_WRITE_BIT,                                               VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {VK_ACCESS_SHADER_READ_BIT,                                                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}},
    {VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, {VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
}}>();

void deferredDelete(size_t idx) {
    for (auto &i : deleteList[idx]) {
        if (i.view.has_value()) vkDestroyImageView(device, i.view.value(), nullptr);
        if (i.image.has_value()) vkDestroyImage(device, i.image.value(), nullptr);
        if (i.framebuffer.has_value()) vkDestroyFramebuffer(device, i.framebuffer.value(), nullptr);
        if (i.memory.has_value()) vkFreeMemory(device, i.memory.value(), nullptr);
        if (i.set.has_value()) vkFreeDescriptorSets(device, descriptorPool, 1, &i.set.value());
    }

    deleteList[idx].clear();
}

void transitionImageLayouts(VkCommandBuffer commandBuffer, std::vector<std::pair<VkImage*, VkImageLayout>> images) {
    std::vector<VkImageMemoryBarrier> barriers;

    VkPipelineStageFlags srcStageMask{};
    VkPipelineStageFlags dstStageMask{};

    for (auto image : images) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = image.first->layout;
        barrier.newLayout = image.second;

        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image.first->image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        auto srcAccess = accessMaskMap[image.first->layout];
        auto dstAccess = accessMaskMap[image.second];

        barrier.srcAccessMask = srcAccess.first;
        barrier.dstAccessMask = dstAccess.first;
        srcStageMask |= srcAccess.second;
        dstStageMask |= dstAccess.second;

        image.first->layout = image.second;

        barriers.push_back(barrier);
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        srcStageMask, dstStageMask,
        0,
        0, nullptr,
        0, nullptr,
        barriers.size(), barriers.data()
    );
}

bool inRecording = false;
void beginCommandBuffer() {
    if (!inRecording) { // Start recording the command buffer
        vkWaitForFences(device, 1, &imageFences[imageBufferIdx], VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &imageFences[imageBufferIdx]);

        vkResetCommandBuffer(imageCommandBuffers[imageBufferIdx], 0);

        // Delete the oldest buffer's delete list.
        deferredDelete(imageBufferIdx);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(imageCommandBuffers[imageBufferIdx], &beginInfo);
        inRecording = true;
    }
}

void VkImage::TransitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout) {
    transitionImageLayouts(commandBuffer, {{this, newLayout}});
}

bool inRenderpass = false;
int cachedDrawMode = -1;
void endRenderPass() {
    if(inRenderpass) {
        vkCmdEndRenderPass(imageCommandBuffers[imageBufferIdx]);
        inRenderpass = false;
    }
}

auto begin_time = std::chrono::high_resolution_clock::now();
void flushCommandBuffer() {
    if (inRecording) {
        endRenderPass();

        vkEndCommandBuffer(imageCommandBuffers[imageBufferIdx]);
        cachedDrawMode = -1;
        inRecording = false;

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &imageCommandBuffers[imageBufferIdx];

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, imageFences[imageBufferIdx]);

        imageBufferIdx = (imageBufferIdx + 1) % NUM_IMAGE_SWAPS; 
    }
}

void VkImage::applyEffects(bool theDoSanding, bool thePremultiply, bool theAlreadyPremultiplied) {
    renderMutex.lock();

    endRenderPass();

    ::VkImage newImage;
    VkDeviceMemory newMemory;
    createImage(
        mWidth, mHeight,
        newImage, newMemory,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT
    );
    
    { // Transition the newly created image to general
        constexpr auto oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        constexpr auto newLayout = VK_IMAGE_LAYOUT_GENERAL;
        const auto srcAccess = accessMaskMap[oldLayout];
        const auto dstAccess = accessMaskMap[newLayout];
        VkImageMemoryBarrier barrier{
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, nullptr,
            srcAccess.first, dstAccess.first,
            oldLayout, newLayout,
            0, 0,
            newImage,
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}
        };

        vkCmdPipelineBarrier(
            imageCommandBuffers[imageBufferIdx],
            srcAccess.second, dstAccess.second,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }

    // Transition the old image to general
    TransitionLayout(imageCommandBuffers[imageBufferIdx], VK_IMAGE_LAYOUT_GENERAL);

    VkImageView newView = createImageView(newImage, pixelFormat);
    { // Update descriptors
        VkDescriptorImageInfo srcImageInfo{
            textureSampler,
            view,
            VK_IMAGE_LAYOUT_GENERAL
        };

        VkDescriptorImageInfo dstImageInfo{
            textureSampler,
            newView,
            VK_IMAGE_LAYOUT_GENERAL
        };

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{{
            {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr,
            computeDescriptorSet, 0, 0,
            1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            &srcImageInfo, nullptr, nullptr},
            {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr,
            computeDescriptorSet, 1, 0,
            1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            &dstImageInfo, nullptr, nullptr},
        }};

        vkUpdateDescriptorSets(
            device,
            descriptorWrites.size(),
            descriptorWrites.data(),
            0,
            nullptr
        );
    }
    
    vkCmdBindPipeline(imageCommandBuffers[imageBufferIdx], VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
    vkCmdBindDescriptorSets(
        imageCommandBuffers[imageBufferIdx],
        VK_PIPELINE_BIND_POINT_COMPUTE,
        computePipelineLayout,
        0, 1,
        &computeDescriptorSet,
        0, 0
    );

    ComputePushConstants constants = {
        theDoSanding,
        thePremultiply,
        theAlreadyPremultiplied
    };

    vkCmdPushConstants(
        imageCommandBuffers[imageBufferIdx],
        computePipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT,
        0, sizeof(ComputePushConstants), &constants
    );

    vkCmdDispatch(imageCommandBuffers[imageBufferIdx], mWidth/16, mHeight/16, 1);

    flushCommandBuffer();
    //vkDeviceWaitIdle(device);

    VkFramebuffer newFramebuffer = CreateFramebuffer();
    std::swap(image, newImage);
    std::swap(view, newView);
    std::swap(framebuffer, newFramebuffer);
    std::swap(memory, newMemory);

    layout = VK_IMAGE_LAYOUT_GENERAL;

    deleteList[imageBufferIdx].emplace_back(
        deleteInfo{newImage, newView, newFramebuffer, newMemory, {}}
    );

    UpdateDescriptorSets();

    renderMutex.unlock();
}

void VkImage::SetViewportAndScissor(const glm::vec4& theClipRect) {
    VkViewport viewport = {
    0, 0,
    static_cast<float>(mWidth),
    static_cast<float>(mHeight),
    0.0, 1.0
    };
    vkCmdSetViewport(imageCommandBuffers[imageBufferIdx], 0, 1, &viewport);

    VkRect2D scissor = {
        {static_cast<int32_t>(theClipRect.x), static_cast<int32_t>(theClipRect.y)},
        {(uint32_t)(theClipRect.z), (uint32_t)(theClipRect.w)}
    };
    vkCmdSetScissor(imageCommandBuffers[imageBufferIdx], 0, 1, &scissor);
}

void VkImage::BeginDraw(Image* theImage, int theDrawMode) {
    static VkImage* otherCachedImage = nullptr;
    static VkImage* thisCachedImage = nullptr;

    bool otherCacheMiss = false;
    bool otherLayoutSuboptimal = false;
    VkImage *otherImage = nullptr;

    otherImage = dynamic_cast<VkImage*>(theImage);
    otherCacheMiss = (otherImage != otherCachedImage);
    otherCachedImage = otherImage;
    otherLayoutSuboptimal = (otherImage->layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    bool thisCacheMiss = (this != thisCachedImage);
    bool drawModeMiss = (theDrawMode != cachedDrawMode);
    cachedDrawMode = theDrawMode;
    thisCachedImage = this;

    bool thisLayoutSuboptimal =  (layout != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    beginCommandBuffer();

    if (drawModeMiss) {
        if (theDrawMode == 1) {
            vkCmdBindPipeline(
                imageCommandBuffers[imageBufferIdx],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                graphicsPipelineAdditive
            );
        }
        else {
            vkCmdBindPipeline(
                imageCommandBuffers[imageBufferIdx],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                graphicsPipeline
            );
        }
    }

    if (thisCacheMiss || otherCacheMiss)
        endRenderPass();

    if (thisLayoutSuboptimal || otherLayoutSuboptimal) {
        std::vector<std::pair<VkImage*, VkImageLayout>> transitions;

        if (thisLayoutSuboptimal) transitions.push_back({this, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
        if (otherLayoutSuboptimal) transitions.push_back({otherImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL});

        transitionImageLayouts(imageCommandBuffers[imageBufferIdx], transitions);
    }

    if (!inRenderpass) {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = imagePass;
        renderPassInfo.framebuffer = framebuffer;

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {
            static_cast<uint32_t>(mWidth),
            static_cast<uint32_t>(mHeight),
        };
        vkCmdBeginRenderPass(imageCommandBuffers[imageBufferIdx], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        inRenderpass = true;

        vkCmdBindDescriptorSets(imageCommandBuffers[imageBufferIdx],
         VK_PIPELINE_BIND_POINT_GRAPHICS,
          pipelineLayout,
          0, 1,
          &otherImage->descriptor,
          0, nullptr
        );
    }
}

/*================*
 | DRAW FUNCTIONS |
 *================*/

glm::vec4 RectToVec4(Rect a) {
    return glm::vec4(a.mX, a.mY, a.mWidth, a.mHeight);
}

std::unique_ptr<VkImage> blankImage;
void VkImage::FillRect(const Rect& theRect, const Color& theColor, int theDrawMode) {
    if (blankImage == nullptr) {
        auto bits = std::make_unique<uint32_t[]>(1);
        bits[0] = 0xFFFFFFFF;
        ImageLib::Image inputImage = ImageLib::Image(1, 1, std::move(bits));
        blankImage = std::make_unique<VkImage>(inputImage);
    }

    Rect theSrcRect = {0, 0, theRect.mWidth, theRect.mHeight};
    BltF(blankImage.get(), 0, 0, theSrcRect, theRect, theColor, theDrawMode);
}

void VkImage::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode){
    Rect theClipRect = {
        0,
        0,
        mWidth,
        mHeight
    };

    BltF(theImage, theX, theY, theSrcRect, theClipRect, theColor, theDrawMode);
}

inline constexpr glm::vec4 calcUVs(const glm::vec4& theSrcRect, const glm::vec2& extent) {
    return glm::vec4(
        (theSrcRect.x)/extent.x,
        (theSrcRect.y)/extent.y,
        (theSrcRect.x + theSrcRect.z)/extent.x,
        (theSrcRect.y + theSrcRect.w)/extent.y
    );
}

constexpr inline std::array<glm::vec4, 4> generateVertices(glm::vec4 dr, glm::vec4 uv, glm::vec2 ex) {
    return {{
        {2 * ((dr.x      ))  / ex.x - 1, 2*((dr.y      ))  / ex.y - 1, uv.x, uv.y},
        {2 * ((dr.x + dr.z)) / ex.x - 1, 2*((dr.y      ))  / ex.y - 1, uv.z, uv.y},
        {2 * ((dr.x      ))  / ex.x - 1, 2*((dr.y + dr.w)) / ex.y - 1, uv.x, uv.w},
        {2 * ((dr.x + dr.z)) / ex.x - 1, 2*((dr.y + dr.w)) / ex.y - 1, uv.z, uv.w},
    }};
}

void VkImage::BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect &theClipRect, const Color& theColor, int theDrawMode){
    BltEx(
        theImage,
        generateVertices(
            {theX, theY, theSrcRect.mWidth, theSrcRect.mHeight},
            calcUVs(RectToVec4(theSrcRect), glm::vec2(theImage->mWidth, theImage->mHeight)),
            {mWidth, mHeight}
        ),
        RectToVec4(theClipRect),
        theColor,
        theDrawMode,
        true
    );
}

void VkImage::StretchBlt(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool) {
    BltEx(
        theImage,
        generateVertices(
            RectToVec4(theDestRect),
            calcUVs(RectToVec4(theSrcRect), glm::vec2(theImage->mWidth, theImage->mHeight)),
            {mWidth, mHeight}
        ),
        RectToVec4(theClipRect),
        theColor,
        theDrawMode,
        true
    );
}

void VkImage::BltMatrix(Image* theImage, float x, float y, const SexyMatrix3 &theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, bool blend){
    glm::mat3 matrix = glm::translate(glm::mat3(
        theMatrix.m00, theMatrix.m10, theMatrix.m20,
        theMatrix.m01, theMatrix.m11, theMatrix.m21,
        theMatrix.m02, theMatrix.m12, theMatrix.m22
    ), glm::vec2(x, y));

    float w2 = theSrcRect.mWidth/2.0;
    float h2 = theSrcRect.mHeight/2.0;

    glm::vec4 uv = calcUVs(RectToVec4(theSrcRect), glm::vec2(theImage->mWidth, theImage->mHeight));

    std::array<glm::vec4, 4> vertices = {{
        {-w2, -h2, uv.x, uv.y},
        { w2, -h2, uv.z, uv.y},
        {-w2,  h2, uv.x, uv.w},
        { w2,  h2, uv.z, uv.w},
    }};

    for (auto &vert : vertices) {
        glm::vec3 v(vert.x, vert.y, 1);
        v = matrix*v;
        vert.x = 2*(v.x/mWidth)  - 1;
        vert.y = 2*(v.y/mHeight) - 1;
    }

    BltEx(theImage, vertices, RectToVec4(theClipRect), theColor, theDrawMode, blend);
}

glm::vec4 colorToVec4(const Color& theColor) {
    float a = theColor.mAlpha/255.0;
    return glm::vec4(
        a*theColor.mRed/255.0,
        a*theColor.mGreen/255.0,
        a*theColor.mBlue/255.0,
        a
    );
}

void VkImage::BltEx(Image* theImage, const std::array<glm::vec4, 4> theVertices, const glm::vec4& theClipRect, const Color& theColor, int theDrawMode, bool blend) {
    if (theClipRect.z <= 0 || theClipRect.w <= 0) return; // Can't draw regions with negative size.
    renderMutex.lock();

    VkImage::BeginDraw(theImage, theDrawMode);

    SetViewportAndScissor(theClipRect);

    auto color = colorToVec4(theColor);
    ImagePushConstants constants = {
        {
            theVertices[0],
            theVertices[1],
            theVertices[2],
            theVertices[3]
        },
        {color, color, color, color},
        true,
        blend
    };

    vkCmdPushConstants(imageCommandBuffers[imageBufferIdx], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ImagePushConstants), &constants);

    vkCmdDraw(imageCommandBuffers[imageBufferIdx], 6,1,0,0);

    renderMutex.unlock();
}

void VkImage::BltTrianglesTex(Image *theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color &theColor, int theDrawMode, float tx, float ty, bool blend){
    if (theClipRect.mWidth <= 0 || theClipRect.mHeight <= 0) return; // Can't draw regions with negative size.
    renderMutex.lock();

    VkImage::BeginDraw(theTexture, theDrawMode);

    SetViewportAndScissor(RectToVec4(theClipRect));

    for (int i = 0; i < theNumTriangles; ++i) {
        auto &triangle = theVertices[i];

        auto vertexToNative = [tx, ty, &mWidth = mWidth, &mHeight = mHeight] (const TriVertex &v) {
            return glm::vec4(
                2*(v.x + tx)/mWidth - 1,
                2*(v.y + ty)/mHeight - 1,
                v.u,
                v.v
            );
        };

        auto colorFromInt = [theColor] (uint32_t c) {
            return colorToVec4(c ? Color(c) : theColor);
        };
        
        ImagePushConstants constants = {
            {
                vertexToNative(triangle[0]),
                vertexToNative(triangle[1]),
                vertexToNative(triangle[2]),
                glm::vec4(0)
            },
            {
                colorFromInt(triangle[0].color),
                colorFromInt(triangle[1].color),
                colorFromInt(triangle[2].color),
                glm::vec4(0)
            },
            false,
            blend
        };

        vkCmdPushConstants(imageCommandBuffers[imageBufferIdx], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ImagePushConstants), &constants);
        vkCmdDraw(imageCommandBuffers[imageBufferIdx], 3,1,0,0);
    }

    renderMutex.unlock();
}

/*=================*
 |  UNIMPLEMENTED  |
 *=================*/

void VkImage::BltRotated(Image*, float, float, const Rect &, const Rect&, const Color&, int, double, float, float){
    static bool has_shown = false;
    if (!has_shown) printf("draw:     VkImage::BltRotated is a stub.\n");
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

void VkImage::FillScanLinesWithCoverage(Span*, int, const Color&, int, const unsigned char*, int, int, int, int){
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
