#ifndef __VK_INTERFACE_H__
#define __VK_INTERFACE_H__

#include "graphics/WindowInterface.h"
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VkInterface : public WindowInterface<VkInterface> {
public:
    VkInterface(int width, int height);
    ~VkInterface();

    bool IsFocused();
    bool ShouldClose();
    void ReleaseMouseCapture();
    void Draw();
};

#endif // __VK_INTERFACE_H__
