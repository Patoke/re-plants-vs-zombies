################################################################################
# SDL external projects
################################################################################

include(ExternalProject)

if(MSVC)
    set(sdl_binary "SDL2.lib")
    set(sdl_image_binary "SDL2_image.lib")
else() # linux
    set(sdl_binary "libSDL2.a")
    set(sdl_image_binary "libSDL2_image.lib")
endif()

set(sdl_dir "${CMAKE_BINARY_DIR}/sdl")
ExternalProject_Add(sdl-src
    URL                     https://www.libsdl.org/release/SDL2-2.28.5.tar.gz
    SOURCE_DIR              "${sdl_dir}"
    BINARY_DIR              "${sdl_dir}/build"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${sdl_dir}/install"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/sdl/;
    COMMAND             ${CMAKE_COMMAND} -E make_directory ${sdl_dir}/build;
    #COMMAND             ${CMAKE_COMMAND} -E copy ${sdl_dir}/include/GLFW/glfw3.h
    #                                             ${sdl_dir}/include/GLFW/glfw3native.h
    #                                        ${CMAKE_BINARY_DIR}/include/glfw/
    BUILD_BYPRODUCTS        "${sdl_dir}/build/${sdl_binary}"
)

set(sdl_image_dir "${CMAKE_BINARY_DIR}/sdl_image")
ExternalProject_Add(sdl_image-src
    GIT_REPOSITORY          https://github.com/libsdl-org/SDL_image.git
    GIT_TAG                 release-2.8.1
    DEPENDS                 sdl-src
    SOURCE_DIR              "${sdl_image_dir}"
    BINARY_DIR              "${sdl_image_dir}/build"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${sdl_image_dir}/install"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DSDL2_CONFIG=${sdl_dir}/build/sdl2-config"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/sdl_image/;
    COMMAND             ${CMAKE_COMMAND} -E make_directory ${sdl_image_dir}/build;
    #COMMAND             ${CMAKE_COMMAND} -E copy ${sdl_dir}/include/GLFW/glfw3.h
    #                                             ${sdl_dir}/include/GLFW/glfw3native.h
    #                                        ${CMAKE_BINARY_DIR}/include/glfw/
    BUILD_BYPRODUCTS        "${sdl_image_dir}/build/${sdl_image_binary}"
)

add_library(sdl STATIC IMPORTED)
add_library(sdl_image STATIC IMPORTED)

add_dependencies(sdl sdl-src)
add_dependencies(sdl_image sdl_image-src)

set_target_properties(sdl
    PROPERTIES IMPORTED_LOCATION "${sdl_dir}/build/${sdl_binary}"
)

set_target_properties(sdl_image
    PROPERTIES IMPORTED_LOCATION "${sdl_image_dir}/build/${sdl_image_binary}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE sdl sdl_image)
add_dependencies(${PROJECT_NAME} sdl-src sdl_image-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${sdl_dir}"
   "${sdl_image_dir}"
)

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Color.h
    GLUtil.cpp
    GLUtil.h
    GLInterface.cpp
    GLInterface.h
    GLImage.cpp
    GLImage.h
    Font.cpp
    Font.h
    Graphics.cpp
    Graphics.h
    Image.cpp
    ImageFont.cpp
    ImageFont.h
    Image.h
    inc_routines
    MemoryImage.cpp
    MemoryImage.h
    NativeDisplay.cpp
    NativeDisplay.h
    Quantize.cpp
    Quantize.h
    SharedImage.cpp
    SharedImage.h
    SWTri.cpp
    SWTri.h
    SysFont.cpp
    SysFont.h
    inc_routines/DDI_Additive.inc
    inc_routines/DDI_AlphaBlt.inc
    inc_routines/DDI_BltRotated_Additive.inc
    inc_routines/DDI_BltRotated.inc
    inc_routines/DDI_FastBlt_NoAlpha.inc
    inc_routines/DDI_FastStretch_Additive.inc
    inc_routines/DDI_FastStretch.inc
    inc_routines/DDI_NormalBlt_Volatile.inc
    inc_routines/GENERIC_DrawLineAA.inc
    inc_routines/MI_AdditiveBlt.inc
    inc_routines/MI_BltRotated_Additive.inc
    inc_routines/MI_BltRotated.inc
    inc_routines/MI_GetNativeAlphaData.inc
    inc_routines/MI_GetRLAlphaData.inc
    inc_routines/MI_NormalBlt.inc
    inc_routines/MI_SlowStretchBlt.inc
)
