include(ExternalProject)

if(WIN32)
    set (glfw_use_wayland OFF)
else()
    set (glfw_use_wayland ON)
endif()

set(libglfw-dir "${CMAKE_BINARY_DIR}/libglfw")
ExternalProject_Add(glfw-src
    GIT_REPOSITORY      https://github.com/glfw/glfw.git
    #GIT_TAG             3.3.9
    SOURCE_DIR          "${libglfw-dir}"
    BINARY_DIR          "${libglfw-dir}"
    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                        "-DCMAKE_C_FLAGS=${universal_cflags}"
                        "-DCMAKE_INSTALL_PREFIX=${libglfw-dir}/install"
                        "-DGLFW_BUILD_DOCS=OFF"
                        "-DGLFW_BUILD_TESTS=OFF"
                        "-DGLFW_BUILD_EXAMPLES=OFF"
                        "-DGLFW_BUILD_X11=OFF"
                        #"-DGLFW_BUILD_WAYLAND=OFF"
                        "-DGLFW_BUILD_WAYLAND=${glfw_use_wayland}"
                        #"-DGLFW_USE_WAYLAND=ON"
                        "-Wno-dev"

    BUILD_BYPRODUCTS    "${libglfw-dir}/install/lib/libglfw3.a"

    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_UPDATE TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD TRUE
    UPDATE_DISCONNECTED TRUE
)
add_library(glfw STATIC IMPORTED)
add_dependencies(glfw glfw-src)
set_target_properties(glfw
    PROPERTIES IMPORTED_LOCATION "${libglfw-dir}/install/lib/libglfw3.a"
)
add_dependencies(${PROJECT_NAME} glfw-src)
target_include_directories(${PROJECT_NAME} PUBLIC ${libglfw-dir}/install/include)

ExternalProject_Add(glm
    PREFIX ${CMAKE_BINARY_DIR}/glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
                    <SOURCE_DIR>/glm ${CMAKE_BINARY_DIR}/include/glm

    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_UPDATE TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD TRUE
    UPDATE_DISCONNECTED TRUE
)

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
