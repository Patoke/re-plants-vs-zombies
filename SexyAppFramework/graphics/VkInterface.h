#ifndef __VK_INTERFACE_H__
#define __VK_INTERFACE_H__

#include "Common.h"

#include "graphics/WindowInterface.h"
#include "widget/WidgetManager.h"
#include <glm/fwd.hpp>
#include <memory>

namespace Vk {

class VkInterface : public WindowInterface<VkInterface> {
public:
    VkInterface(int width, int height, WidgetManager* mWidgetManager);
    ~VkInterface();

    Image* GetScreenImage();
    int  CreateCursor(int xHotSpot, int yHotSpot, int nWidth, int nHeight, const void* pvANDPlane, const void* pvXORPlane);
    void EnforceCursor();
    void ShowWindow();
    void PollEvents();
    bool IsFocused();
    void RehupFocus();
    bool ShouldClose();
    void ReleaseMouseCapture();
    void Draw();
};

} // namespace Vk

#endif // __VK_INTERFACE_H__
