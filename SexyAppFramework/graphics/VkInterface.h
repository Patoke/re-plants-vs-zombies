#ifndef __VK_INTERFACE_H__
#define __VK_INTERFACE_H__

#include "graphics/WindowInterface.h"
#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vk {

class VkInterface : public WindowInterface<VkInterface> {
public:
    VkInterface(int width, int height);
    ~VkInterface();

    Image* GetScreenImage();
    void ShowWindow();
    void PollEvents();
    bool IsFocused();
    bool ShouldClose();
    void ReleaseMouseCapture();
    void Draw();
};

} // namespace Vk

#endif // __VK_INTERFACE_H__
