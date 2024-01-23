target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(${PROJECT_NAME} PRIVATE
    "Common.cpp"
    "SexyApp.cpp"
    "SexyAppBase.cpp"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:Common.h>"
)

add_subdirectory(graphics)
add_subdirectory(sound)
add_subdirectory(widget)
add_subdirectory(misc)
add_subdirectory(paklib)
add_subdirectory(imagelib)
