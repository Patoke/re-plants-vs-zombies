target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "RELEASEFINAL;"
    ">"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_BINARY_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src"
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    LawnApp.cpp
    Resources.cpp
)

add_subdirectory(framework)
add_subdirectory(todlib)
add_subdirectory(lawn)
add_subdirectory(res)
