#include "VkInterface.h"
#include "SexyAppBase.h"
#include "GLFW/glfw3.h"
#include "VkCommon.h"
#include "graphics/VkImage.h"
#include "graphics/WindowInterface.h"
#include "misc/KeyCodes.h"
#include "widget/WidgetManager.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>
#include <chrono>
#include <vulkan/vulkan_core.h>

#ifdef __cplusplus
extern "C"
#endif
const char* __asan_default_options() { return "detect_leaks=0"; }

#define DECLARE_SHADER(NAME)\
    extern const uint8_t NAME [];\
    extern const size_t NAME ## _size;

#define CREATE_SHADER_MODULE(NAME) createShaderModule(NAME, NAME ## _size)

DECLARE_SHADER(_binary_sanding_comp_spv)
DECLARE_SHADER(_binary_shader_frag_spv)
DECLARE_SHADER(_binary_shader_vert_spv)

namespace Vk {

const int MAX_FRAMES_IN_FLIGHT = 3;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

/*===================================*
 | Function forward declaration      |
 *===================================*/
void createSyncObjects();
void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
void createDescriptorSetLayouts();

void createImage(
    uint32_t width, uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    ::VkImage& image,
    VkDeviceMemory& imageMemory
);

VkImageView createImageView(::VkImage image, VkFormat format);
void createTextureSampler();
void createTextureImageView();

void createBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory
);

void createDescriptorSets();
void createDescriptorPool();

VkCommandBuffer beginSingleTimeCommands();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
void createCommandBuffers();

void copyBufferToImage(VkBuffer buffer, ::VkImage image, uint32_t width, uint32_t height);
void createTextureImage(int width, int height, uint32_t const *imdata);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

void createCommandPool();
void createRenderPass();

VkShaderModule createShaderModule(const uint8_t *code, const size_t length);

void createComputePipeline();
void createGraphicsPipelines();
void createImageViews();
void createFramebuffers();

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

void createSwapChain();
void cleanupSwapChain();
void recreateSwapChain();
void createSurface();
void createLogicalDevice();

bool checkDeviceExtensionSupport(VkPhysicalDevice device);
int rateDeviceSuitability(VkPhysicalDevice device);
void pickPhysicalDevice();

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    [[maybe_unused]] void* pUserData
);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
);
void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
);
void setupDebugMessenger();

bool checkValidationLayerSupport();
std::vector<const char*> getRequiredExtensions();
void createInstance();

static void framebufferResizeCallback(GLFWwindow*, int, int);
static void windowFocusCallback(GLFWwindow*, int focused);

void SetCursor(int idx);

/*===================================*
 | File scope declarations           |
 *===================================*/
GLFWwindow* window;
WidgetManager* widgetManager;
glm::vec<2, double> cursorPos;

VkInstance instance;

VkDebugUtilsMessengerEXT debugMessenger;

VkPhysicalDevice physicalDevice;
VkPhysicalDeviceProperties physicalDeviceProperties;
VkDevice device;

VkQueue imageQueue;
VkQueue graphicsQueue;
VkQueue presentQueue;

VkSurfaceKHR surface;

VkSwapchainKHR swapChain;
std::vector<::VkImage> swapChainImages;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
std::vector<VkImageView> swapChainImageViews;
std::vector<VkFramebuffer> swapChainFramebuffers;

VkRenderPass renderPass;
VkRenderPass imagePass;

VkDescriptorPool descriptorPool;
std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets;
VkDescriptorSet computeDescriptorSet;
VkDescriptorSetLayout descriptorSetLayout;
VkDescriptorSetLayout computeDescriptorSetLayout;

VkPipelineLayout pipelineLayout;
VkPipelineLayout computePipelineLayout;
VkPipeline graphicsPipeline;
VkPipeline graphicsPipelineAdditive;
VkPipeline computePipeline;

VkCommandPool commandPool;
std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> commandBuffers;
std::array<VkCommandBuffer, NUM_IMAGE_SWAPS> imageCommandBuffers;

VkSampler textureSampler;

std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> imageAvailableSemaphores;
std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> renderFinishedSemaphores;
std::array<VkFence, MAX_FRAMES_IN_FLIGHT> inFlightFences;
std::array<VkFence, NUM_IMAGE_SWAPS> imageFences;

std::mutex renderMutex;

VkImage* windowImage;
glm::vec4 windowImageClipRect;
double windowImageScale;

bool framebufferResized = false;

uint32_t currentFrame = 0;

class glfwCursor {
public:
    GLFWcursor *cursor;
    glfwCursor(GLFWimage *image, int xhot, int yhot) {
        cursor = glfwCreateCursor(image, xhot, yhot);
    }
    glfwCursor(int shape) {
        cursor = glfwCreateStandardCursor(shape);
        standard = true;
    }
    ~glfwCursor() {
        if (!standard) glfwDestroyCursor(cursor);
    }
private:
    bool standard = false;
};
std::map<int, std::unique_ptr<glfwCursor>> cursorMap;

template <decltype(auto) arr>
static constexpr auto const_generate_keymap() {
    using T = decltype(arr)::value_type::second_type;

    constexpr auto max_key = std::max_element(arr.begin(), arr.end(),
        [](const auto &left, const auto &right) {
            return left.first < right.first;
        })->first;

    std::array<T, max_key + 1> sparse_array{};

    for (auto it : arr) {
        sparse_array[it.first] = it.second;
    }

    for (size_t key = 0; key < arr.size(); ++key) {
        if (!sparse_array[key]) {
            int offset = key;
            if      (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) offset = 0;
            else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) offset = 0;
            else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9) offset = GLFW_KEY_KP_0 - KEYCODE_NUMPAD0;
            else if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F24) offset = GLFW_KEY_F1 - KEYCODE_F1;
            sparse_array[key] = (KeyCode)(key - offset); // if we didn't find it the resulting keycode is key - key == 0
        }
    }

    return sparse_array;
}

constexpr auto keymap = const_generate_keymap<std::array<std::pair<uint16_t, KeyCode>, 33> {{
    {GLFW_KEY_BACKSPACE,     KEYCODE_BACK},
    {GLFW_KEY_TAB,           KEYCODE_TAB},
    {GLFW_KEY_ENTER,         KEYCODE_RETURN},
    {GLFW_KEY_PAUSE,         KEYCODE_PAUSE},
    {GLFW_KEY_CAPS_LOCK,    KEYCODE_CAPITAL},
    {GLFW_KEY_ESCAPE,       KEYCODE_ESCAPE},
    {GLFW_KEY_SPACE,        KEYCODE_SPACE},
    {GLFW_KEY_PAGE_UP,      KEYCODE_PRIOR},
    {GLFW_KEY_PAGE_DOWN,    KEYCODE_NEXT},
    {GLFW_KEY_END,          KEYCODE_END},
    {GLFW_KEY_HOME,         KEYCODE_HOME},
    {GLFW_KEY_LEFT,         KEYCODE_LEFT},
    {GLFW_KEY_UP,           KEYCODE_UP},
    {GLFW_KEY_RIGHT,        KEYCODE_RIGHT},
    {GLFW_KEY_DOWN,         KEYCODE_DOWN},
    {GLFW_KEY_PRINT_SCREEN, KEYCODE_SNAPSHOT},
    {GLFW_KEY_INSERT,       KEYCODE_INSERT},
    {GLFW_KEY_DELETE,       KEYCODE_DELETE},
    {GLFW_KEY_LEFT_SUPER,   KEYCODE_LWIN},
    {GLFW_KEY_RIGHT_SUPER,  KEYCODE_RWIN},
    {GLFW_KEY_KP_MULTIPLY,  KEYCODE_MULTIPLY},
    {GLFW_KEY_KP_ADD,       KEYCODE_ADD},
    {GLFW_KEY_KP_SUBTRACT,  KEYCODE_SUBTRACT},
    {GLFW_KEY_KP_DECIMAL,   KEYCODE_DECIMAL},
    {GLFW_KEY_KP_DIVIDE,    KEYCODE_DIVIDE},
    {GLFW_KEY_NUM_LOCK,     KEYCODE_NUMLOCK},
    {GLFW_KEY_SCROLL_LOCK,  KEYCODE_SCROLL},
    {GLFW_KEY_LEFT_SHIFT,   KEYCODE_LSHIFT},
    {GLFW_KEY_RIGHT_SHIFT,  KEYCODE_RSHIFT},
    {GLFW_KEY_LEFT_CONTROL, KEYCODE_LCONTROL},
    {GLFW_KEY_RIGHT_CONTROL,KEYCODE_RCONTROL},
    {GLFW_KEY_LEFT_ALT,     KEYCODE_LMENU},
    {GLFW_KEY_RIGHT_ALT,    KEYCODE_RMENU},
}}>();

/*===================================*
 | Implementations                   |
 *===================================*/

void createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }

    for (size_t i = 0; i < NUM_IMAGE_SWAPS; i++) {
        if (vkCreateFence(device, &fenceInfo, nullptr, &imageFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create imageFence");
        }
    }
}

void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    windowImage->TransitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {static_cast<int32_t>(windowImageClipRect.x), static_cast<int32_t>(windowImageClipRect.y)};
    renderPassInfo.renderArea.extent = {static_cast<uint32_t>(windowImageClipRect.z), static_cast<uint32_t>(windowImageClipRect.w)};
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = windowImageClipRect.x;
    viewport.y = windowImageClipRect.y;
    viewport.width = windowImageClipRect.z;
    viewport.height = windowImageClipRect.w;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = renderPassInfo.renderArea.offset;
    scissor.extent = renderPassInfo.renderArea.extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    ImagePushConstants constants = {
        {
            glm::vec4(-1, -1, 0.0, 0.0),
            glm::vec4(-1, 1, 0.0, 1.0),
            glm::vec4(1, -1, 1.0, 0.0),
            glm::vec4(1, 1, 1.0, 1.0),
        },
        {
            glm::vec4(1.0),
            glm::vec4(1.0),
            glm::vec4(1.0),
            glm::vec4(1.0)
        },
        true,
        true,
    };

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ImagePushConstants), &constants);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
    vkCmdDraw(commandBuffer, 6,1,0,0);
    vkCmdEndRenderPass(commandBuffer);
    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    static bool readProperties = false;
    static VkPhysicalDeviceMemoryProperties memProperties;
    if (!readProperties) {
        readProperties = true;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    }

    auto checkProperties = [typeFilter, properties] (uint32_t i) {
        return typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
    };

    // leveraging the fact that the previous property
    // is likely to be the same as the current one.
    static uint32_t previousOutput = 0;
    if (checkProperties(previousOutput)) return previousOutput;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (checkProperties(i)) {
            previousOutput = i;
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void createDescriptorSetLayouts() {
    {
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 0;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 1> bindings = {samplerLayoutBinding};

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }
    
    {
        std::array<VkDescriptorSetLayoutBinding, 2> computeLayoutBindings{{
            {
                0,
                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                1,
                VK_SHADER_STAGE_COMPUTE_BIT,
                nullptr
            },
            {
                1,
                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                1,
                VK_SHADER_STAGE_COMPUTE_BIT,
                nullptr
            },
        }};

        VkDescriptorSetLayoutCreateInfo layoutInfo{
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            nullptr,
            0,
            computeLayoutBindings.size(),
            computeLayoutBindings.data()
        };

        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &computeDescriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }
}

void createImage(
    uint32_t width, uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    ::VkImage& image,
    VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView createImageView(::VkImage image, VkFormat format) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void createTextureSampler() {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void createDescriptorSets() {
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        
    }

    {
        VkDescriptorSetAllocateInfo allocInfo{
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            nullptr,
            descriptorPool,
            1,
            &computeDescriptorSetLayout
        };

        vkAllocateDescriptorSets(device, &allocInfo, &computeDescriptorSet);
    }

    for (size_t i = 0; i < descriptorSets.size(); i++) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = windowImage->view;
        imageInfo.sampler = textureSampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
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
    }
}

void createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = descriptorSets.size() + 2000;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[1].descriptorCount = 2;

    VkDescriptorPoolCreateInfo poolInfo{
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, nullptr,
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        poolSizes[0].descriptorCount + poolSizes[1].descriptorCount,
        static_cast<uint32_t>(poolSizes.size()),
        poolSizes.data()
    };

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

VkCommandBuffer beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void createCommandBuffers() {
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = commandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate commandd buffers!");
        }
    }{
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = imageCommandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocInfo, imageCommandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate commandd buffers!");
        }
    }
}

void copyBufferToImage(VkBuffer buffer, ::VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    endSingleTimeCommands(commandBuffer);
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices queueIndices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) queueIndices.presentFamily = i;
        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) queueIndices.graphicsAndComputeFamily = i;
        if (queueIndices.isComplete()) break;

        i++;
    }

    return queueIndices;
}

void createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsAndComputeFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void createRenderPass() {
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;


        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &imagePass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }
}

VkShaderModule createShaderModule(const uint8_t *code, const size_t length) {
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = length;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code);

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void createComputePipeline() {
    VkShaderModule computeShaderModule = CREATE_SHADER_MODULE(_binary_sanding_comp_spv);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        nullptr,
        0,
        VK_SHADER_STAGE_COMPUTE_BIT,
        computeShaderModule,
        "main",
        nullptr
    };

    VkPushConstantRange pushConstant;
    pushConstant.offset = 0;
    pushConstant.size = sizeof(ComputePushConstants);
    pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        1,
        &computeDescriptorSetLayout,
        1,
        &pushConstant
    };

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &computePipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute pipeline layout!");
    }

    VkComputePipelineCreateInfo pipelineInfo{
        VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        nullptr,
        0,
        computeShaderStageInfo,
        computePipelineLayout,
        nullptr,
        0,
    };

    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) != VK_SUCCESS) {
        throw std::runtime_error("filed to create compute pipeline!");
    }

    vkDestroyShaderModule(device, computeShaderModule, nullptr);
}

void createGraphicsPipelines() {
    VkShaderModule vertShaderModule = CREATE_SHADER_MODULE(_binary_shader_vert_spv);
    VkShaderModule fragShaderModule = CREATE_SHADER_MODULE(_binary_shader_frag_spv);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    //auto bindingDescription = Vertex::getBindingDescription();
    //auto attributeDescriptions = Vertex::getAttributeDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    //rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPushConstantRange pushConstant;
    pushConstant.offset = 0;
    pushConstant.size = sizeof(ImagePushConstants);
    pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstant;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipelineAdditive) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat);
    }
}

void createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == pixelFormat && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& /*availablePresentModes*/) {
    /*
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        //if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            return availablePresentMode;
        }
    }*/

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices queueIndices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {queueIndices.graphicsAndComputeFamily.value(), queueIndices.presentFamily.value()};

    if (queueIndices.graphicsAndComputeFamily != queueIndices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void cleanupSwapChain() {
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(device, swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void setWindowDimensions() {
    glm::vec2 forceAspect = {4, 3};
    double ratioW = swapChainExtent.width/forceAspect.x;
    double ratioH = swapChainExtent.height/forceAspect.y;
    bool pillarboxed = ratioW > ratioH;
    double newWidth = pillarboxed ? swapChainExtent.height*forceAspect.x/forceAspect.y:swapChainExtent.width;
    double newHeight = pillarboxed ? swapChainExtent.height : swapChainExtent.width*forceAspect.y/forceAspect.x;
    windowImageClipRect.x = pillarboxed ? (swapChainExtent.width - newWidth) / 2.0 : 0;
    windowImageClipRect.y = pillarboxed ? 0 : (swapChainExtent.height - newHeight) / 2.0;
    windowImageClipRect.z = newWidth;
    windowImageClipRect.w = newHeight;
    windowImageScale = pillarboxed ? newWidth/windowImage->mWidth : newHeight/windowImage->mHeight;
    printf("x: %f, y: %f, z: %f, w: %f, scale: %f\n", windowImageClipRect.x, windowImageClipRect.y, windowImageClipRect.z, windowImageClipRect.w, windowImageScale);
}

void createWindowBuffer(int intendedWidth, int intendedHeight) {
    if (windowImage) delete windowImage;
    windowImage = new VkImage(intendedWidth, intendedHeight);

    setWindowDimensions();
}

void recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents(); // Pause the window
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createFramebuffers();
    setWindowDimensions();
}

void createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void createLogicalDevice() {
    QueueFamilyIndices queueIndices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueIndices.graphicsAndComputeFamily.value(), queueIndices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_FALSE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.enabledLayerCount = 0; // Device specific layers no longer exists

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, queueIndices.graphicsAndComputeFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, queueIndices.presentFamily.value(), 0, &presentQueue);
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

int rateDeviceSuitability(VkPhysicalDevice device) {
    {
        QueueFamilyIndices queueIndices = findQueueFamilies(device);
        if (!queueIndices.isComplete()) return 0;
    }

    {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(device, pixelFormat, &formatProperties);
        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)) return 0;
    }

    if (!checkDeviceExtensionSupport(device)) return 0;

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) return 0;

    int score = 0;

    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        switch (deviceProperties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: score = 5; break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score = 4; break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU: score = 3; break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: score = 2; break;
            default: score = 1; break;
        }
    }

    return score;
}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    [[maybe_unused]] void* pUserData) {

    std::cerr << "\033[0;31m" << pCallbackData->pMessage << "\033[0m" << std::endl;

    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        for (const auto& layerProperties : availableLayers)
            if (strcmp(layerName, layerProperties.layerName) == 0) goto layer_found;
        return false;
        layer_found:;
    }

    return true;
}

std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> vk_extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, vk_extensions.data());

    // Check for required extensions
    for (uint32_t i = 0; i < extensions.size(); ++i) {
        for (const auto& vk_extension: vk_extensions)
            if (strcmp(vk_extension.extensionName, extensions[i]) == 0) goto ext_found;
        throw std::runtime_error("Could not find extension required for glfw.");
        ext_found:;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

VkInterface::~VkInterface() {
    flushCommandBuffer();
    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    vkDestroySampler(device, textureSampler, nullptr);

    delete windowImage;

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayout, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    for (size_t i = 0; i < NUM_IMAGE_SWAPS; i++) {
        vkDestroyFence(device, imageFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipeline(device, graphicsPipelineAdditive, nullptr);
    vkDestroyPipeline(device ,computePipeline, nullptr);

    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyPipelineLayout(device, computePipelineLayout, nullptr);

    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroyRenderPass(device, imagePass, nullptr);

    vkDestroyDevice(device, nullptr);
    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

static void framebufferResizeCallback(GLFWwindow*, int, int) {
    framebufferResized = true;
}

static void windowFocusCallback(GLFWwindow* w, int focused) {
    widgetManager->mApp->mActive = focused;

    if (focused) {
        VkInterface* interface = (VkInterface*)glfwGetWindowUserPointer(w);
        interface->RehupFocus();
    
        // Potentially not required
        if ((widgetManager->mApp->mActive) && (!widgetManager->mApp->mIsWindowed))
            widgetManager->MarkAllDirty();
    }
    
}

void VkInterface::RehupFocus()
{
    bool wantHasFocus = widgetManager->mApp->mActive && !widgetManager->mApp->mMinimized;

    if (wantHasFocus != widgetManager->mApp->mHasFocus)
    {
        widgetManager->mApp->mHasFocus = wantHasFocus;

        if (widgetManager->mApp->mHasFocus)
        {

            if (widgetManager->mApp->mMuteOnLostFocus)
                widgetManager->mApp->Unmute(true);
    
            widgetManager->GotFocus();
            widgetManager->mApp->GotFocus();
        }
        else
        {
            if (widgetManager->mApp->mMuteOnLostFocus)
                widgetManager->mApp->Mute(true);
    
            widgetManager->LostFocus();
            widgetManager->mApp->LostFocus();

            ReleaseMouseCapture();
            widgetManager->DoMouseUps();
        }
    }
}


//0x455930
void VkInterface::EnforceCursor()
{
    if (!widgetManager->mApp->mMouseIn)
    {
        SetCursor(CURSOR_DEFAULT);
        return;
    }

    SetCursor(widgetManager->mApp->mCursorNum);
}

static void cursorEnterCallback(GLFWwindow* w, int entered) {
    bool isMouseIn = entered;
    if (widgetManager->mApp->mMouseIn != isMouseIn)
    {
        if (!isMouseIn)
        {
            int x = cursorPos.x;
            int y = cursorPos.y;
            widgetManager->RemapMouse(x, y);
            widgetManager->MouseExit(x, y);
        }

        widgetManager->mApp->mMouseIn = isMouseIn;
        ((VkInterface*)glfwGetWindowUserPointer(w))->EnforceCursor();
    }
}

static void cursorPositionCallback(GLFWwindow* w, double xpos, double ypos) {
    cursorPos = {
        (xpos - windowImageClipRect.x)/windowImageScale,
        (ypos - windowImageClipRect.y)/windowImageScale
    };

    glm::vec<2, int> intPos = cursorPos;
    widgetManager->RemapMouse(intPos.x, intPos.y);
    widgetManager->MouseMove(intPos.x, intPos.y);

    if (!widgetManager->mApp->mMouseIn) {
        widgetManager->mApp->mMouseIn = true;
        VkInterface* theInterface = (VkInterface*)glfwGetWindowUserPointer(w);
        theInterface->EnforceCursor();
    }
}

static void mouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/) {
    /*=====================================*
     | This table maps a GLFW_MOUSE_BUTTON |
     |    to what WidgetManager expects    |
     *=====================================*/
    constexpr int mouseButtonTranslationTable[] = {
        1,  // GLFW_MOUSE_BUTTON_LEFT
        -1, // GLFW_MOUSE_BUTTON_RIGHT
        3,  // GLFW_MOUSE_BUTTON_MIDDLE
    };
    /*=====================================*
     |   These are currently unavailable:  |
     |       Double click left:   2        |
     |       Double click right: -2        |
     *=====================================*/

    int wmButton = mouseButtonTranslationTable[button];
    switch (action) {
    case GLFW_RELEASE:
        widgetManager->MouseUp(cursorPos.x, cursorPos.y,  wmButton);
        break;
    case GLFW_PRESS:
        widgetManager->MouseDown(cursorPos.x, cursorPos.y, wmButton);
        break;
    }
}

void keyCallback(GLFWwindow*, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_UNKNOWN)
		return;

    auto code = keymap[key];

    if (action == GLFW_PRESS) {
        widgetManager->KeyDown(code);
    } else if (action == GLFW_RELEASE) {
        widgetManager->KeyUp(code);
    }
}

void charCallback(GLFWwindow*, uint32_t codepoint) {
    char chr[] = {static_cast<char>(codepoint),
                static_cast<char>(codepoint >> 8),
                static_cast<char>(codepoint >> 16),
                static_cast<char>(codepoint >> 24), 0};
    widgetManager->KeyChar(std::string(chr)[0]); // broken, but one day utf8
}

void windowCloseCallback(GLFWwindow*) {
    widgetManager->mApp->CloseRequestAsync();
}

void initGLFW(int width, int height, VkInterface* userPtr) {
    // Init glfw
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "Plants Vs Zombies", /*glfwGetPrimaryMonitor()*/nullptr, nullptr);
    glfwSetWindowUserPointer(window, userPtr);
    //glfwSetWindowAspectRatio(window, 4, 3);

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetWindowCloseCallback(window, windowCloseCallback);
}

VkInterface::VkInterface(int width, int height, WidgetManager* mWidgetManager) {
    widgetManager = mWidgetManager;
    initGLFW(width, height, this);

    // Init vulkan
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createCommandPool();
    createTextureSampler();
    createDescriptorSetLayouts();
    createDescriptorPool();
    createWindowBuffer(width, height);
    createDescriptorSets();
    createComputePipeline();
    createGraphicsPipelines();
    createFramebuffers();
    //createVertexBuffer();
    //createIndexBuffer(); 
    createCommandBuffers();
    createSyncObjects();
}

void SetCursor(int idx) {
    constexpr int standardCursorLut[] =  {
    GLFW_ARROW_CURSOR,       // CURSOR_POINTER,
    GLFW_HAND_CURSOR,        // CURSOR_HAND,
    GLFW_CROSSHAIR_CURSOR,   // CURSOR_DRAGGING,
    GLFW_IBEAM_CURSOR,       // CURSOR_TEXT,
    GLFW_NOT_ALLOWED_CURSOR, // CURSOR_CIRCLE_SLASH,
    GLFW_RESIZE_ALL_CURSOR,  // CURSOR_SIZEALL,
    GLFW_RESIZE_NESW_CURSOR, // CURSOR_SIZENESW,
    GLFW_RESIZE_NS_CURSOR,   // CURSOR_SIZENS,
    GLFW_RESIZE_NWSE_CURSOR, // CURSOR_SIZENWSE,
    GLFW_RESIZE_EW_CURSOR,   // CURSOR_SIZEWE,
    0,                      // CURSOR_WAIT,
    0,                      // CURSOR_NONE,
    GLFW_ARROW_CURSOR,      // CURSOR_DEFAULT,
    0,                      // CURSOR_CUSTOM,
    };

    if (standardCursorLut[idx] == 0)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    glfwSetCursor(window, cursorMap.insert(
        std::pair(idx, std::make_unique<glfwCursor>(standardCursorLut[idx]))
        ).first->second->cursor
    );
}

int VkInterface::CreateCursor(int xHotSpot, int yHotSpot, int nWidth, int nHeight, const void* pvANDPlane, const void* pvXORPlane) {
    static int cursorMapNewKey = NUM_POINTER_TYPES;

    GLFWimage image{};
    image.width = nWidth;
    image.height = nHeight;
    auto pixels = std::make_unique<uint32_t[]>(nWidth*nHeight);

    for (int i = 0; i < nHeight; ++i) {
        for (int j = 0; j < nWidth; ++j) {
            bool andPixel = (((uint8_t*)pvANDPlane)[(i*nWidth + j)/8] >> ((i*nWidth + j)%8)) & 1;
            bool xorPixel = (((uint8_t*)pvXORPlane)[(i*nWidth + j)/8] >> ((i*nWidth + j)%8)) & 1;
            constexpr uint32_t colorIndex[] = {
                0x000000FF,
                0xFFFFFFFF,
                0x00000000,
                0x00000000,
            };
            pixels[i*nWidth + j] = colorIndex[(andPixel << 1) | xorPixel];
        }
    }

    image.pixels = (unsigned char*)pixels.get();
    cursorMap.insert(std::pair(cursorMapNewKey, std::make_unique<glfwCursor>(&image, xHotSpot, yHotSpot)));

    return cursorMapNewKey++;
}

Image* VkInterface::GetScreenImage() {
    return windowImage;
}

void VkInterface::PollEvents() {
    glfwPollEvents();
}

void VkInterface::ShowWindow() {
    glfwShowWindow(window);
}

bool VkInterface::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void VkInterface::ReleaseMouseCapture() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void VkInterface::Draw() {
    renderMutex.lock();
    flushCommandBuffer();

    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        renderMutex.unlock();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Only reset the fence if we are submitting work
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    renderMutex.unlock();
}

}
