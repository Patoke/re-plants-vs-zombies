target_sources(${PROJECT_NAME} PRIVATE
    "Common.h"
    "Common.cpp"
    "SexyApp.cpp"
    "SexyApp.h"
    "SexyAppBase.cpp"
    "SexyAppBase.h"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "RELEASEFINAL;"
    ">"
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
