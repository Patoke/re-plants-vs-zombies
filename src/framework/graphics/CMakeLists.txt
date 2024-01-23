include(ExternalProject)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan wayland-client)

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Color.h
    Font.cpp
    Graphics.cpp
    Image.cpp
    ImageFont.cpp
    VkInterface.cpp
    VkImage.cpp
)

add_subdirectory(shaders)
