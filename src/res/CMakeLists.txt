################################################################################
# RC
################################################################################
set(RC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)
file(GLOB RC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*
)

list(REMOVE_ITEM RC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt") # Don't include self into the binary

create_resources(${CMAKE_BINARY_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp
)