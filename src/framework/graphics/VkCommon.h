#ifndef VK_COMMON_H
#define VK_COMMON_H

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Vk {

extern VkDevice device;

class VkImage;

constexpr int NUM_IMAGE_SWAPS = 3;

constexpr VkFormat pixelFormat = VK_FORMAT_B8G8R8A8_UNORM;

extern std::mutex renderMutex;

extern VkQueue graphicsQueue;
extern VkPipeline graphicsPipeline;
extern VkPipeline computePipeline;
extern VkPipeline graphicsPipelineAdditive;
extern std::array<VkCommandBuffer, NUM_IMAGE_SWAPS> imageCommandBuffers;
extern VkRenderPass imagePass;
extern std::array<VkFence, NUM_IMAGE_SWAPS> imageFences;

extern VkDescriptorPool descriptorPool;
extern VkDescriptorSetLayout descriptorSetLayout;
extern VkDescriptorSet computeDescriptorSet;

//extern std::vector<VkBuffer> uniformBuffers;
//extern VkBuffer vertexBuffer;
//extern VkBuffer indexBuffer;

extern VkPipelineLayout pipelineLayout;
extern VkPipelineLayout computePipelineLayout;
extern VkSampler textureSampler;

extern uint32_t currentFrame;
//extern VkImage* otherCachedImage;

extern const std::vector<uint16_t> indices;

VkImageView createImageView(::VkImage image, VkFormat format);

struct ImagePushConstants {
    glm::vec4 vertices[4];
    glm::vec4 vertex_colors[4];
    uint32_t  is_quad;
    uint32_t  filter;
    glm::vec2 filterOffset = glm::vec2();
};

struct ComputePushConstants {
    uint32_t sand;
    uint32_t premultiply;
    uint32_t already_premultiplied;
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

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

VkCommandBuffer beginSingleTimeCommands();
void flushCommandBuffer();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);
void copyBufferToImage(VkBuffer buffer, ::VkImage image, uint32_t width, uint32_t height);

} // namespace Vk

#endif // VK_COMMON_H
