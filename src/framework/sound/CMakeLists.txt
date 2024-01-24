include(FetchContent)

target_sources(${PROJECT_NAME}  PRIVATE
    "BassMusicInterface.cpp"
    "BassSoundInstance.cpp"
    "BassSoundManager.cpp"
)


if (WIN32)
    set(BASS_SRC https://www.un4seen.com/files/bass24.zip)
    set(BASS_LIB c/x64/bass.lib)
    set(BASS_INCLUDE c)
elseif (UNIX)
    if (APPLE)
        set(BASS_SRC https://www.un4seen.com/files/bass24-osx.zip)
        set(BASS_LIB libbass.dylib)
    else()
        set(BASS_SRC https://www.un4seen.com/files/bass24-linux.zip)
        set(BASS_LIB libs/x86_64/libbass.a)
    set(BASS_INCLUDE "")
endif()

FetchContent_Declare(
    bass
    URL                     ${BASS_SRC}
    DOWNLOAD_EXTRACT_TIMESTAMP ${CMAKE_CURRENT_BINARY_DIR}/bass-download
)

FetchContent_Populate(bass)


add_library(bass STATIC IMPORTED GLOBAL)
set_target_properties(bass PROPERTIES
    IMPORTED_LOCATION ${bass_SOURCE_DIR}/${BASS_LIB}
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    bass
)

target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${bass_SOURCE_DIR}/${BASS_INCLUDE}
)