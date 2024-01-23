set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
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

make_directory(${SHADER_OUTPUT_DIR})
foreach(SHADER ${SHADERS})
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMENT "Compiling ${FILENAME}")
endforeach()

create_resources(${SHADER_OUTPUT_DIR} ${CMAKE_CURRENT_BINARY_DIR}/shader_resources.cpp)

# add_custom_target( shader ALL DEPENDS ${SHADER_OUTPUT_DIR}/shader_resources.cpp )

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/shader_resources.cpp
)
