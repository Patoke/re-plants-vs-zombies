target_sources(${PROJECT_NAME}  PRIVATE
    "BassMusicInterface.cpp"
    "BassSoundInstance.cpp"
    "BassSoundManager.cpp"
)

add_library(bass SHARED IMPORTED)
set_target_properties(bass
    PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libbass.so
)
target_link_libraries(${PROJECT_NAME} PRIVATE bass)
