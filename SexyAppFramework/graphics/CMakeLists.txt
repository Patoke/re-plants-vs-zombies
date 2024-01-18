include(ExternalProject)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

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

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILENAME}")
    file(RELATIVE_PATH rel ${SHADER_OUTPUT_DIR} ${SHADER_OUTPUT_DIR}/${FILENAME}.spv)
    string( MAKE_C_IDENTIFIER ${rel} input_identifier )
endForeach()

create_resources(${SHADER_OUTPUT_DIR} ${SHADER_OUTPUT_DIR}/shaders.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${SHADER_OUTPUT_DIR}/shaders.cpp
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
