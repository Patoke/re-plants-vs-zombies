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

set(SHADER_DEPENDS "")

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
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SHADER_OUTPUT_DIR}/${input_identifier}.o)

list(APPEND SHADER_DEPENDS ${SHADER_OUTPUT_DIR}/${input_identifier}.o)
endForeach()

add_custom_target( shader ALL DEPENDS ${SHADER_DEPENDS} )
