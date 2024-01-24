################################################################################
# Project definition
################################################################################


target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(${PROJECT_NAME} PRIVATE
    "Common.cpp"
    "SexyApp.cpp"
    "SexyAppBase.cpp"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:Common.h>"
)

add_subdirectory(graphics)
add_subdirectory(sound)
add_subdirectory(widget)
add_subdirectory(misc)
add_subdirectory(paklib)
add_subdirectory(imagelib)


################################################################################
# SDL external projects
################################################################################

set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

include(FetchContent)
FetchContent_Declare(
    zlib
    GIT_REPOSITORY          https://github.com/madler/zlib.git
    GIT_TAG                 v1.3
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS            TRUE
    GIT_SHALLOW             TRUE
)
FetchContent_MakeAvailable(zlib)

FetchContent_Declare(
    glfw3
    GIT_REPOSITORY      https://github.com/glfw/glfw.git
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS            TRUE
    GIT_SHALLOW             TRUE
)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
if(UNIX AND NOT APPLE)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(glfw3)

set(HAVE_SDL_LOADSO TRUE)
set(HAVE_SDL_THREADS TRUE)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY                     https://github.com/libsdl-org/SDL.git
    GIT_TAG                            release-2.30.x
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS                       TRUE
    GIT_SHALLOW                        TRUE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

include_directories(${SDL2_SOURCE_DIR}/include)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY          https://github.com/libsdl-org/SDL_image.git
    GIT_TAG                 release-2.8.2
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS            TRUE
    GIT_SHALLOW             TRUE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY          https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG                 release-2.6.3
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS            TRUE
    GIT_SHALLOW             TRUE
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_mixer)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    # TODO: Set this to "0.9.9.9" when glm 0.9.9.9 releases in a few days.
    # graphics/VkImage.cpp: Remove #define GLM_ENABLE_EXPERIMENTAL when this happens
    #GIT_TAG        "0.9.9.8"
    OVERRIDE_FIND_PACKAGE
    GIT_PROGRESS            TRUE
    GIT_SHALLOW             TRUE
    SYSTEM
)

FetchContent_MakeAvailable(glm)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    SDL2::SDL2-static 
    SDL2_image::SDL2_image-static
    SDL2_mixer::SDL2_mixer-static
    zlibstatic
    glfw
    glm::glm
)
# Adds include directories
target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${SDL2_image_SOURCE_DIR}/include
    ${SDL2_mixer_SOURCE_DIR}/include
    ${zlib_SOURCE_DIR}
    ${zlib_BINARY_DIR}
    ${glfw_SOURCE_DIR}/include
)
