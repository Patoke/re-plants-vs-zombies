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

make_directory(${SHADER_OUTPUT_DIR})
foreach(SHADER ${SHADERS})
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_OUTPUT_DIR}/${FILENAME}.spv
        COMMENT "Compiling ${FILENAME}"
        DEPENDS ${SHADER})
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/${FILENAME}.spv.cpp
        COMMAND ${CMAKE_COMMAND} -Dbin="${SHADER_OUTPUT_DIR}/${FILENAME}.spv" -Doutput="${SHADER_OUTPUT_DIR}/${FILENAME}.spv.cpp" -P "${CMAKE_SOURCE_DIR}/cmake/embed_binary.cmake"
        COMMENT "Converting ${FILENAME} to C++"
        DEPENDS ${SHADER_OUTPUT_DIR}/${FILENAME}.spv)
    list(APPEND SHADER_DEPENDS ${SHADER_OUTPUT_DIR}/${FILENAME}.spv.cpp)
endforeach()

add_custom_target(shaders DEPENDS ${SHADER_DEPENDS})
add_library(shaders_lib STATIC ${SHADER_DEPENDS})

target_link_libraries(${PROJECT_NAME} PRIVATE shaders_lib)