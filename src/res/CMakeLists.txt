################################################################################
# RC
################################################################################
set(RC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)
file(GLOB RC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*
)

list(REMOVE_ITEM RC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt") # Don't include self into the binary

set(RC_DEPENDS "")

foreach(FILE IN LISTS RC_SRC)
    get_filename_component(FILENAME ${FILE} NAME)
    string( MAKE_C_IDENTIFIER ${FILENAME} input_identifier )
    set(outfile ${RC_OUTPUT_DIR}/${input_identifier}.o)
    add_custom_command(
        OUTPUT ${outfile}
        COMMAND ${CMAKE_LINKER} -r --format=binary -m elf_amd64 -o ${outfile} -z noexecstack ${FILENAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PUBLIC ${outfile})
list(APPEND RC_DEPENDS ${outfile})
endforeach()

add_custom_target(rc ALL DEPENDS ${RC_DEPENDS})
add_dependencies(${PROJECT_NAME} rc)
