#ifndef __VK_IMAGE_H__
#define __VK_IMAGE_H__

#include <memory>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Image.h"
#include "imagelib/ImageLib.h"

using namespace Sexy;

namespace Vk {

class VkImage : public Image
{
public:
    VkImage(ImageLib::Image &theImage);
    VkImage(int width, int height) : VkImage(*std::make_unique<ImageLib::Image>(width, height)) {}
    ~VkImage();

    bool mHasTrans = false;
    bool mHasAlpha = false;

    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    ::VkImage image;
    VkImageView view;
    VkDeviceMemory memory;

    void TransitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout);

    void FillRect(const Rect&, const Color&, int);
    void ClearRect(const Rect&);
    void DrawLine(double, double, double, double, const Color&, int);
    void DrawLineAA(double, double, double, double, const Color&, int);
    void FillScanLinesWithCoverage(Span*, int, const Color&, int, const BYTE*, int, int, int, int);
    bool PolyFill3D(const Point*, int, const Rect*, const Color&,int ,int, int);
    void Blt(Image*, int, int, const Rect&, const Color&, int);
    void BltF(Image*, float, float, const Rect&, const Rect&, const Color&, int);
    void BltRotated(Image*, float, float, const Rect &, const Rect&, const Color&, int, double, float, float);
    void StretchBlt(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool);
    void BltMatrix(Image*, float, float, const SexyMatrix3&, const Rect&, const Color&, int, const Rect&, bool);
    void BltTrianglesTex(Image*, const TriVertex (*)[3], int, const Rect&, const Color &, int, float, float, bool);
    void BltMirror(Image*, int, int, const Rect&, const Color&, int);
    void StretchBltMirror(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool);
};

} // namespace Vk

#endif // __VK_IMAGE_H__
