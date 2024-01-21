#include(ExternalProject)
#
# support for windows
#if(MSVC)
#    set(ogg_binary "ogg-static.lib")
#else() # linux
#    set(ogg_binary "libogg.a")
#endif()
#
#set(ogg_dir "${CMAKE_BINARY_DIR}/ogg")
#ExternalProject_Add(ogg-src
#    GIT_REPOSITORY      https://github.com/xiph/ogg.git
#    GIT_TAG             v1.3.5
#    SOURCE_DIR          "${ogg_dir}"
#    BINARY_DIR          "${ogg_dir}"
#    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
#                        "-DCMAKE_INSTALL_PREFIX=${ogg_dir}/install" # unused, have to specify
#                        "-DCMAKE_C_FLAGS=${universal_cflags}"
#                        "-Wno-dev"
#
#    INSTALL_COMMAND     ""
#    #INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/jpeg/;
#    #COMMAND             ${CMAKE_COMMAND} -E copy ${ogg_dir}/jpeglib.h
#    #                                             ${ogg_dir}/jerror.h
#    #                                             ${ogg_dir}/jconfig.h
#    #                                             ${ogg_dir}/jmorecfg.h
#    #                                             ${ogg_dir}/jpegint.h
#    #                                        ${CMAKE_BINARY_DIR}/include/jpeg/
#    BUILD_BYPRODUCTS    "${ogg_dir}/${ogg_binary}"
#    USES_TERMINAL_DOWNLOAD TRUE
#    USES_TERMINAL_UPDATE TRUE
#    USES_TERMINAL_CONFIGURE TRUE
#    USES_TERMINAL_BUILD TRUE
#    UPDATE_DISCONNECTED TRUE
#)
#
#add_library(ogg STATIC IMPORTED)
#add_dependencies(ogg ogg-src)
#set_target_properties(ogg
#    PROPERTIES IMPORTED_LOCATION "${ogg_dir}/${ogg_binary}"
#)
#
#target_link_libraries(${PROJECT_NAME} PRIVATE ogg)
#add_dependencies(${PROJECT_NAME} ogg-src) # Ensures headers exist


#set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
#   "${ogg_dir}"
#)

target_sources(${PROJECT_NAME} PRIVATE
    #"DSoundManager.h"
    #"FModLoader.h"
    #"FModMusicInterface.h"
    #"FModSoundInstance.h"
    #"FModSoundManager.h"
    #"DSoundInstance.cpp"
    #"DSoundManager.cpp"
    #"FModLoader.cpp"
    #"FModMusicInterface.cpp"
    #"FModSoundInstance.cpp"
    #"FModSoundManager.cpp"
    #"SoundBuild.cpp"

    "MusicInterface.h"
    "SoundInstance.h"
    "SoundManager.h"

    "DummyMusicInterface.h"
    "DummySoundManager.h"
    "DummySoundInstance.h"

    "SDLMusicInterface.h"
    "SDLMusicInterface.cpp"

    "BassSoundInstance.h"
    "SDLSoundInstance.cpp"

    "BassSoundManager.h"
    "BassSoundManager.cpp"

    "BassLoader.h"
    "BassLoader.cpp"
)
