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

find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw Vulkan::Vulkan wayland-client)

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(GLOB SHADERS
    ${SHADER_DIR}/*.vert
    ${SHADER_DIR}/*.frag
    ${SHADER_DIR}/*.comp
    ${SHADER_DIR}/*.geom
    ${SHADER_DIR}/*.tesc
    ${SHADER_DIR}/*.tese
    ${SHADER_DIR}/*.mesh
    ${SHADER_DIR}/*.task
    ${SHADER_DIR}/*.rgen
    ${SHADER_DIR}/*.rchit
    ${SHADER_DIR}/*.rmiss
)

set( SHADER_DEPENDS "" )

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILENAME}")
    file(RELATIVE_PATH rel ${SHADER_OUTPUT_DIR} ${SHADER_OUTPUT_DIR}/${FILENAME}.spv)
    string( MAKE_C_IDENTIFIER ${rel} input_identifier )
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/${input_identifier}.o
        COMMAND ${CMAKE_LINKER} -r --format=binary -m elf_amd64 -o ${input_identifier}.o -z noexecstack ${FILENAME}.spv
        DEPENDS ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        WORKING_DIRECTORY ${SHADER_OUTPUT_DIR}
    )
    target_link_libraries( ${PROJECT_NAME} PRIVATE ${SHADER_OUTPUT_DIR}/${input_identifier}.o )

list(APPEND SHADER_DEPENDS ${SHADER_OUTPUT_DIR}/${input_identifier}.o)
endForeach()

add_custom_target( shader ALL DEPENDS ${SHADER_DEPENDS} )

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Color.h
#    D3D8Helper.cpp
#    D3D8Helper.h
#    D3DInterface.cpp
#    D3DInterface.h
#    D3DTester.cpp
#    D3DTester.h
#    DDImage.cpp
#    DDImage.h
#    DDInterface.cpp
#    DDInterface.h
    Font.cpp
    Font.h
    Graphics.cpp
    Graphics.h
    Image.cpp
    ImageFont.cpp
    ImageFont.h
    Image.h
    inc_routines
#    MemoryImage.cpp
#    MemoryImage.h
#    NativeDisplay.cpp
#    NativeDisplay.h
    Quantize.cpp
    Quantize.h
#    SharedImage.cpp
#    SharedImage.h
#    SWTri.cpp
#    SWTri.h
#    SysFont.cpp
#    SysFont.h
#    inc_routines/DDI_Additive.inc
#    inc_routines/DDI_AlphaBlt.inc
#    inc_routines/DDI_BltRotated_Additive.inc
#    inc_routines/DDI_BltRotated.inc
#    inc_routines/DDI_FastBlt_NoAlpha.inc
#    inc_routines/DDI_FastStretch_Additive.inc
#    inc_routines/DDI_FastStretch.inc
#    inc_routines/DDI_NormalBlt_Volatile.inc
#    inc_routines/GENERIC_DrawLineAA.inc
#    inc_routines/MI_AdditiveBlt.inc
#    inc_routines/MI_BltRotated_Additive.inc
#    inc_routines/MI_BltRotated.inc
#    inc_routines/MI_GetNativeAlphaData.inc
#    inc_routines/MI_GetRLAlphaData.inc
#    inc_routines/MI_NormalBlt.inc
#    inc_routines/MI_SlowStretchBlt.inc

    WindowInterface.h
    VkInterface.h
    VkInterface.cpp
    VkImage.cpp
    VkImage.h
)
