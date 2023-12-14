################################################################################
# glfw external project
################################################################################

include(ExternalProject)

if(MSVC)
    set(glfw_binary "glfw3.lib")
else() # linux
    set(glfw_binary "glfw3.a") # @Patoke todo: is this the actual name output for linux?
endif()

set(glfw_dir "${CMAKE_BINARY_DIR}/glfw")
ExternalProject_Add(glfw-src
    GIT_REPOSITORY          https://github.com/glfw/glfw.git
    GIT_TAG                 3.3.8
    SOURCE_DIR              "${glfw_dir}"
    INSTALL_DIR             "${glfw_dir}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${glfw_dir}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DGLFW_BUILD_DOCS=OFF"
                            "-DGLFW_BUILD_TESTS=OFF"
                            "-DGLFW_BUILD_EXAMPLES=OFF"
    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/glfw/;
    COMMAND             ${CMAKE_COMMAND} -E copy ${glfw_dir}/include/GLFW/glfw3.h
                                                 ${glfw_dir}/include/GLFW/glfw3native.h
                                            ${CMAKE_BINARY_DIR}/include/glfw/
    BUILD_BYPRODUCTS        "${glfw_dir}/src/${glfw_binary}"
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(glfw STATIC IMPORTED GLOBAL)

add_dependencies(glfw glfw-src)

set_target_properties(glfw
    PROPERTIES IMPORTED_LOCATION "${glfw_dir}/src/${glfw_binary}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
add_dependencies(${PROJECT_NAME} glfw-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${glfw_dir}"
)

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Color.h
    D3D8Helper.cpp
    D3D8Helper.h
    D3DInterface.cpp
    D3DInterface.h
    D3DTester.cpp
    D3DTester.h
    DDImage.cpp
    DDImage.h
    DDInterface.cpp
    DDInterface.h
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
