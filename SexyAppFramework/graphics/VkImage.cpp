#include "VkImage.h"
#include "Common.h"
#include "VkCommon.h"
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace Vk {

// imageArray is a registry of all images loaded into the GPU
std::vector<VkImage*> imageArray;

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
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
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
    imageArray.push_back(this); // Add this image resource to the registry
    
    renderMutex.unlock();
}

VkImage::~VkImage() {
    renderMutex.lock();
    vkDestroyImageView(device, view, nullptr);
    vkDestroyImage(device, image, nullptr);
    vkFreeMemory(device, memory, nullptr);
    renderMutex.unlock();

    auto position = std::find(imageArray.begin(), imageArray.end(), this);
    if (position != imageArray.end()) imageArray.erase(position);
    else unreachable(); // Expected to find ourselves in the registry.
}

void VkImage::TransitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout) {
    transitionImageLayout(commandBuffer, image, layout, newLayout);
    layout = newLayout;
}

/*================*
 | DRAW FUNCTIONS |
 *================*/

void VkImage::FillRect(const Rect& /*theRect*/, const Color& /*theColor*/, int /*theDrawMode*/){
    renderMutex.lock();
    /*
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    auto color = VkClearColorValue{ .float32 = {theColor.mRed/255.0f, theColor.mGreen/255.0f, theColor.mBlue/255.0f} };

    auto subresourceRange = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdClearColorImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &subresourceRange);

    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    endSingleTimeCommands(commandBuffer);
    */
    renderMutex.unlock();
}

void VkImage::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color&/*theColor*/, int/*theDrawMode*/){
    renderMutex.lock();

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkImage *vkImage = dynamic_cast<VkImage*>(theImage);
    vkImage->TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    VkImageSubresourceLayers subresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};

    VkOffset3D srcOffsets[2] = {{theSrcRect.mX,theSrcRect.mY,0}, {theSrcRect.mWidth, theSrcRect.mHeight, 1}};
    VkOffset3D dstOffsets[2] = {{theX, theY, 0}, {theSrcRect.mWidth, theSrcRect.mHeight, 1}};
    VkImageBlit pRegions{};
    pRegions.srcSubresource = subresource;
    pRegions.dstSubresource = subresource;
    memcpy(pRegions.srcOffsets, srcOffsets, sizeof(srcOffsets));
    memcpy(pRegions.dstOffsets, dstOffsets, sizeof(dstOffsets));

    vkCmdBlitImage(
        commandBuffer,
        vkImage->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &pRegions,
        VK_FILTER_NEAREST
    );

    TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    endSingleTimeCommands(commandBuffer);

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
