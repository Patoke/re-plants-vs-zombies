################################################################################
# RC
################################################################################
set(RC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)
file(GLOB RC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*
)

list(REMOVE_ITEM RC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt") # Don't include self into the binary

set(RC_DEPENDS "")
foreach(RC_FILE ${RC_SRC})
    get_filename_component(RC_FILE_NAME ${RC_FILE} NAME)
    add_custom_command(
        OUTPUT ${RC_OUTPUT_DIR}/${RC_FILE_NAME}.cpp
        COMMAND ${CMAKE_COMMAND} -Dbin="${RC_FILE}" -Doutput="${RC_OUTPUT_DIR}/${RC_FILE_NAME}.cpp" -P "${CMAKE_SOURCE_DIR}/cmake/embed_binary.cmake"
        DEPENDS ${RC_FILE}
        COMMENT "Converting ${RC_FILE_NAME} to C++"
    )
    list(APPEND RC_DEPENDS ${RC_OUTPUT_DIR}/${RC_FILE_NAME}.cpp)
endforeach()

add_custom_target(rc DEPENDS ${RC_DEPENDS})
add_library(rc_lib STATIC ${RC_DEPENDS})
target_link_libraries(${PROJECT_NAME} PRIVATE rc_lib)