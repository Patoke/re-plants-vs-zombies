################################################################################
# Utility functions
################################################################################

# https://stackoverflow.com/a/27206982
# Creates C resources file from files in given directory
function(create_resources dir output)
    # Create empty output file
    file(WRITE ${output} "")
    # Collect input files
    file(GLOB bins ${dir}/*)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output} "extern const char _binary_${filename}[] = {${filedata}};\nextern const size_t _binary_${filename}_size = sizeof(_binary_${filename});\n\n")
    endforeach()
endfunction()

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

include(FetchContent)

FetchContent_Declare(
    zlib
    GIT_REPOSITORY          https://github.com/madler/zlib.git
    GIT_TAG                 v1.3
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(zlib)

FetchContent_Declare(
    glfw3
    GIT_REPOSITORY      https://github.com/glfw/glfw.git
    OVERRIDE_FIND_PACKAGE
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

FetchContent_Declare(
    SDL2
    URL                     https://www.libsdl.org/release/SDL2-2.28.5.tar.gz
    OVERRIDE_FIND_PACKAGE
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
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY          https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG                 release-2.6.3
    OVERRIDE_FIND_PACKAGE
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_mixer)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    SDL2::SDL2-static 
    SDL2_image::SDL2_image-static
    SDL2_mixer::SDL2_mixer-static
    zlibstatic
    glfw
)
# Adds include directories
target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${SDL2_image_SOURCE_DIR}/include
    ${SDL2_mixer_SOURCE_DIR}/include
    ${zlib_SOURCE_DIR}
    ${glfw_SOURCE_DIR}/include
)