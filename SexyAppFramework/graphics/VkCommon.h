#ifndef VK_COMMON_H
#define VK_COMMON_H

#include <memory>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Vk {

extern VkDevice device;

class VkImage;

extern std::vector<Vk::VkImage*> imageArray;
extern std::mutex renderMutex;

VkImageView createImageView(::VkImage image, VkFormat format);

void createBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory
);

void createImage(
    uint32_t width, uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    ::VkImage& image,
    VkDeviceMemory& imageMemory
);

VkCommandBuffer beginSingleTimeCommands();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);
void transitionImageLayout(VkCommandBuffer commandBuffer, ::VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(VkBuffer buffer, ::VkImage image, uint32_t width, uint32_t height);

} // namespace Vk

#endif // VK_COMMON_H
