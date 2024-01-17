#ifndef VK_COMMON_H
#define VK_COMMON_H

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Vk {

extern VkDevice device;

class VkImage;

constexpr int num_image_swaps = 2;

extern std::mutex renderMutex;

extern VkQueue graphicsQueue;
extern VkPipeline graphicsPipeline;
extern VkPipeline graphicsPipelineAdditive;
extern std::array<VkCommandBuffer, num_image_swaps> imageCommandBuffers;
extern VkRenderPass imagePass;
extern VkFence imageFence;

extern VkDescriptorPool descriptorPool;
extern VkDescriptorSetLayout descriptorSetLayout;

//extern std::vector<VkBuffer> uniformBuffers;
//extern VkBuffer vertexBuffer;
//extern VkBuffer indexBuffer;

extern VkPipelineLayout pipelineLayout;
extern VkSampler textureSampler;

extern uint32_t currentFrame;
//extern VkImage* otherCachedImage;

extern const std::vector<uint16_t> indices;

VkImageView createImageView(::VkImage image, VkFormat format);

struct ImagePushConstants {
    glm::vec4 vertices[4];
    uint32_t  vertex_colors[4];
    glm::vec4 render_color;
    uint32_t  is_quad;
    uint32_t  filter;
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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
void flushCommandBuffer();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);
void transitionImageLayout(VkCommandBuffer commandBuffer, ::VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(VkBuffer buffer, ::VkImage image, uint32_t width, uint32_t height);

} // namespace Vk

#endif // VK_COMMON_H
