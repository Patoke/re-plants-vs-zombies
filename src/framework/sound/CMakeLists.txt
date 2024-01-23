include(FetchContent)

target_sources(${PROJECT_NAME}  PRIVATE
    "BassMusicInterface.cpp"
    "BassSoundInstance.cpp"
    "BassSoundManager.cpp"
)


if (WIN32)
    set(BASS_SRC https://www.un4seen.com/files/bass24.zip)
    set(BASS_LIB bass.lib)
elseif (APPLE)
    set(BASS_SRC https://www.un4seen.com/files/bass24-osx.zip)
    set(BASS_LIB libbass.dylib)
elseif (UNIX)
    set(BASS_SRC https://www.un4seen.com/files/bass24-linux.zip)
    set(BASS_LIB libbass.a)
endif()

FetchContent_Declare(
    bass
    URL                     ${BASS_SRC}
    DOWNLOAD_EXTRACT_TIMESTAMP ${CMAKE_CURRENT_BINARY_DIR}/bass-download
)

FetchContent_Populate(bass)

add_library(bass STATIC IMPORTED GLOBAL)
set_target_properties(bass PROPERTIES
    IMPORTED_LOCATION ${bass_SOURCE_DIR}/c/x64/${BASS_LIB}
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    bass
)