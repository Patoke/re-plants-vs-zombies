set(PROJECT_NAME LawnProject)


################################################################################
# Utility functions
################################################################################

# https://stackoverflow.com/a/27206982
# Creates C resources file from files in given directory
function(create_resources dir output)
    # Create empty output file
    file(WRITE ${output} "")
    # Collect input files
    file(GLOB bins ${dir}/*)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output} "extern const char _binary_${filename}[] = {${filedata}};\nextern const size_t _binary_${filename}_size = sizeof(_binary_${filename});\n\n")
    endforeach()
endfunction()

################################################################################
# Source groups
################################################################################

set(Lawn__Board__Board_Include
    "../Lawn/Board.h"
    "../Lawn/Challenge.h"
    "../Lawn/Coin.h"
    "../Lawn/CursorObject.h"
    "../Lawn/Cutscene.h"
    "../Lawn/GameObject.h"
    "../Lawn/GridItem.h"
    "../Lawn/LawnMower.h"
    "../Lawn/MessageWidget.h"
    "../Lawn/Plant.h"
    "../Lawn/Projectile.h"
    "../Lawn/SeedPacket.h"
    "../Lawn/ToolTipWidget.h"
    "../Lawn/ZenGarden.h"
    "../Lawn/Zombie.h"
)
source_group("Lawn\\Board\\Board Include" FILES ${Lawn__Board__Board_Include})

set(Lawn__Board__Board_Source
    "../Lawn/Board.cpp"
    "../Lawn/Challenge.cpp"
    "../Lawn/Coin.cpp"
    "../Lawn/CursorObject.cpp"
    "../Lawn/CutScene.cpp"
    "../Lawn/GameObject.cpp"
    "../Lawn/GridItem.cpp"
    "../Lawn/LawnMower.cpp"
    "../Lawn/MessageWidget.cpp"
    "../Lawn/Plant.cpp"
    "../Lawn/Projectile.cpp"
    "../Lawn/SeedPacket.cpp"
    "../Lawn/ToolTipWidget.cpp"
    "../Lawn/ZenGarden.cpp"
    "../Lawn/Zombie.cpp"
)
source_group("Lawn\\Board\\Board Source" FILES ${Lawn__Board__Board_Source})

set(Lawn__Common__Common_Include
    "../ConstEnums.h"
    "../GameConstants.h"
    "../Lawn/BoardInclude.h"
    "../Lawn/LawnCommon.h"
    "../Resources.h"
)
source_group("Lawn\\Common\\Common Include" FILES ${Lawn__Common__Common_Include})

set(Lawn__Common__Common_Source
    "../Lawn/LawnCommon.cpp"
    "../Resources.cpp"
)
source_group("Lawn\\Common\\Common Source" FILES ${Lawn__Common__Common_Source})

set(Lawn__System__System_Include
    "../Lawn/System/DataSync.h"
    "../Lawn/System/Music.h"
    "../Lawn/System/PlayerInfo.h"
    "../Lawn/System/PoolEffect.h"
#   "../Lawn/System/PopDRMComm.h"
    "../Lawn/System/ProfileMgr.h"
    "../Lawn/System/ReanimationLawn.h"
    "../Lawn/System/SaveGame.h"
    "../Lawn/System/TypingCheck.h"
)
source_group("Lawn\\System\\System Include" FILES ${Lawn__System__System_Include})

set(Lawn__System__System_Source
    "../Lawn/System/DataSync.cpp"
    "../Lawn/System/Music.cpp"
    "../Lawn/System/PlayerInfo.cpp"
    "../Lawn/System/PoolEffect.cpp"
#   "../Lawn/System/PopDRMComm.cpp"
    "../Lawn/System/ProfileMgr.cpp"
    "../Lawn/System/ReanimationLawn.cpp"
    "../Lawn/System/SaveGame.cpp"
    "../Lawn/System/TypingCheck.cpp"
)
source_group("Lawn\\System\\System Source" FILES ${Lawn__System__System_Source})

set(Lawn__Widget__Widget_Include
    "../Lawn/Widget/AchievementsScreen.h"
    "../Lawn/Widget/AlmanacDialog.h"
    "../Lawn/Widget/AwardScreen.h"
    "../Lawn/Widget/ChallengeScreen.h"
    "../Lawn/Widget/CheatDialog.h"
    "../Lawn/Widget/ContinueDialog.h"
    "../Lawn/Widget/CreditScreen.h"
    "../Lawn/Widget/GameButton.h"
    "../Lawn/Widget/GameSelector.h"
    "../Lawn/Widget/ImitaterDialog.h"
    "../Lawn/Widget/LawnDialog.h"
    "../Lawn/Widget/NewOptionsDialog.h"
    "../Lawn/Widget/NewUserDialog.h"
    "../Lawn/Widget/SeedChooserScreen.h"
    "../Lawn/Widget/StoreScreen.h"
    "../Lawn/Widget/TitleScreen.h"
    "../Lawn/Widget/UserDialog.h"
)
source_group("Lawn\\Widget\\Widget Include" FILES ${Lawn__Widget__Widget_Include})

set(Lawn__Widget__Widget_Source
    "../Lawn/Widget/AchievementsScreen.cpp"
    "../Lawn/Widget/AlmanacDialog.cpp"
    "../Lawn/Widget/AwardScreen.cpp"
    "../Lawn/Widget/ChallengeScreen.cpp"
    "../Lawn/Widget/CheatDialog.cpp"
    "../Lawn/Widget/ContinueDialog.cpp"
    "../Lawn/Widget/CreditScreen.cpp"
    "../Lawn/Widget/GameButton.cpp"
    "../Lawn/Widget/GameSelector.cpp"
    "../Lawn/Widget/ImitaterDialog.cpp"
    "../Lawn/Widget/LawnDialog.cpp"
    "../Lawn/Widget/NewOptionsDialog.cpp"
    "../Lawn/Widget/NewUserDialog.cpp"
    "../Lawn/Widget/SeedChooserScreen.cpp"
    "../Lawn/Widget/StoreScreen.cpp"
    "../Lawn/Widget/TitleScreen.cpp"
    "../Lawn/Widget/UserDialog.cpp"
)
source_group("Lawn\\Widget\\Widget Source" FILES ${Lawn__Widget__Widget_Source})

set(TodLib__Common__TodCommon_Include
    "../Sexy.TodLib/DataArray.h"
    "../Sexy.TodLib/Definition.h"
    "../Sexy.TodLib/TodCommon.h"
    "../Sexy.TodLib/TodDebug.h"
    "../Sexy.TodLib/TodFoley.h"
    "../Sexy.TodLib/TodList.h"
    "../Sexy.TodLib/TodStringFile.h"
)
source_group("TodLib\\Common\\TodCommon Include" FILES ${TodLib__Common__TodCommon_Include})

set(TodLib__Common__TodCommon_Source
    "../Sexy.TodLib/Definition.cpp"
    "../Sexy.TodLib/TodCommon.cpp"
    "../Sexy.TodLib/TodDebug.cpp"
    "../Sexy.TodLib/TodFoley.cpp"
    "../Sexy.TodLib/TodList.cpp"
    "../Sexy.TodLib/TodStringFile.cpp"
)
source_group("TodLib\\Common\\TodCommon Source" FILES ${TodLib__Common__TodCommon_Source})

set(TodLib__Effect__Effect_Include
    "../Sexy.TodLib/Attachment.h"
    "../Sexy.TodLib/EffectSystem.h"
    "../Sexy.TodLib/FilterEffect.h"
    "../Sexy.TodLib/ReanimAtlas.h"
    "../Sexy.TodLib/Reanimator.h"
    "../Sexy.TodLib/TodParticle.h"
    "../Sexy.TodLib/Trail.h"
)
source_group("TodLib\\Effect\\Effect Include" FILES ${TodLib__Effect__Effect_Include})

set(TodLib__Effect__Effect_Source
    "../Sexy.TodLib/Attachment.cpp"
    "../Sexy.TodLib/EffectSystem.cpp"
    "../Sexy.TodLib/FilterEffect.cpp"
    "../Sexy.TodLib/ReanimAtlas.cpp"
    "../Sexy.TodLib/Reanimator.cpp"
    "../Sexy.TodLib/TodParticle.cpp"
    "../Sexy.TodLib/Trail.cpp"
)
source_group("TodLib\\Effect\\Effect Source" FILES ${TodLib__Effect__Effect_Source})

set(ALL_FILES
    ${no_group_source_files}
    ${Lawn__Board__Board_Include}
    ${Lawn__Board__Board_Source}
    ${Lawn__Common__Common_Include}
    ${Lawn__Common__Common_Source}
    ${Lawn__System__System_Include}
    ${Lawn__System__System_Source}
    ${Lawn__Widget__Widget_Include}
    ${Lawn__Widget__Widget_Source}
    ${TodLib__Common__TodCommon_Include}
    ${TodLib__Common__TodCommon_Source}
    ${TodLib__Effect__Effect_Include}
    ${TodLib__Effect__Effect_Source}
)


################################################################################
# Target
################################################################################
if(MSVC)
    add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
else()
    add_executable(${PROJECT_NAME} ${ALL_FILES})
endif()

################################################################################
# SDL external projects
################################################################################

include(ExternalProject)


if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(sdl_binary "SDL2-staticd.lib")
        set(sdl_main_binary "SDL2maind.lib")
        set(sdl_image_binary "SDL2_image-staticd.lib")
    else()
        set(sdl_binary "SDL2-static.lib")
        set(sdl_main_binary "SDL2main.lib")
        set(sdl_image_binary "SDL2_image-static.lib")
    endif()
else() # linux
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(sdl_binary "libSDL2d.a")
        set(sdl_main_binary "libSDL2maind.a")
        set(sdl_image_binary "libSDL2_imaged.a")
    else()
        set(sdl_binary "libSDL2.a")
        set(sdl_main_binary "libSDL2main.a")
        set(sdl_image_binary "libSDL2_image.lib")
    endif()
endif()

set(sdl_dir "${CMAKE_SOURCE_DIR}/Dependencies/SDL")
set(sdl_build "${CMAKE_BINARY_DIR}/SDL")
set(SDL2_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/SDL2")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SDL2_EXTRA_HEADER "${sdl_build}/include-config-debug/SDL2/SDL_config.h")
else()
    set(SDL2_EXTRA_HEADER "${sdl_build}/include-config-release/SDL2/SDL_config.h")
endif()

ExternalProject_Add(sdl-src
    URL                     https://www.libsdl.org/release/SDL2-2.28.5.tar.gz
    SOURCE_DIR              "${sdl_dir}"
    BINARY_DIR              "${sdl_build}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DSDL_SHARED=OFF"
                            "-DSDL_TEST=OFF"
                            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"

    BUILD_BYPRODUCTS        "${sdl_build}/${sdl_binary}"
                            "${sdl_build}/${sdl_main_binary}"
                            "${SDL2_INCLUDE_DIR}/"
    
    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${SDL2_INCLUDE_DIR};
    COMMAND             ${CMAKE_COMMAND} -E copy_directory ${sdl_dir}/include ${SDL2_INCLUDE_DIR};
    COMMAND             ${CMAKE_COMMAND} -E copy ${SDL2_EXTRA_HEADER} ${SDL2_INCLUDE_DIR};

    DOWNLOAD_EXTRACT_TIMESTAMP true
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

set(sdl_image_dir "${CMAKE_SOURCE_DIR}/Dependencies/SDL_image")
set(sdl_image_build "${CMAKE_BINARY_DIR}/SDL_image")
ExternalProject_Add(sdl_image-src
    GIT_REPOSITORY          https://github.com/libsdl-org/SDL_image.git
    GIT_TAG                 release-2.8.2
    DEPENDS                 sdl-src
    SOURCE_DIR              "${sdl_image_dir}"
    BINARY_DIR              "${sdl_image_build}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DSDL2_CONFIG=${sdl_build}/sdl2-config"
                            "-DSDL2_LIBRARY=${sdl_build}/${sdl_binary}"
                            "-DSDL2_INCLUDE_DIR=${SDL2_INCLUDE_DIR}"
                            "-DSDL2_MAIN_LIBRARY=${sdl_build}/${sdl_main_binary}"
                            "-DBUILD_SHARED_LIBS=OFF"
                            "-DBUILD_TESTING=OFF"
                            "-DSDL2IMAGE_SAMPLES=OFF"
                            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E copy ${sdl_image_dir}/include/SDL_image.h ${SDL2_INCLUDE_DIR};
    BUILD_BYPRODUCTS        "${sdl_image_build}/${sdl_image_binary}"
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(sdl STATIC IMPORTED)
add_library(sdl_image STATIC IMPORTED)

add_dependencies(sdl sdl-src)
add_dependencies(sdl_image sdl_image-src)

set_target_properties(sdl
    PROPERTIES IMPORTED_LOCATION "${sdl_build}/${sdl_binary}"
)

set_target_properties(sdl_image
    PROPERTIES IMPORTED_LOCATION "${sdl_image_build}/${sdl_image_binary}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE sdl sdl_image)
add_dependencies(${PROJECT_NAME} sdl-src sdl_image-src) # Ensures headers exist

set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
   "${sdl_build}"
   "${sdl_image_build}"
)

################################################################################
# Subdirectories
################################################################################

add_subdirectory(graphics)
add_subdirectory(sound)
add_subdirectory(widget)
add_subdirectory(misc)
add_subdirectory(paklib)
#add_subdirectory(ogg)
add_subdirectory(imagelib)

target_sources(${PROJECT_NAME} PRIVATE
    "Common.h"
    "Common.cpp"
    "SexyApp.cpp"
    "SexyApp.h"
    "SexyAppBase.cpp"
    "SexyAppBase.h"

    # No group sources
    #"../Cursor13.cur"
    "../LawnApp.cpp"
    "../LawnApp.h"
    "../main.cpp"
    ".gitattributes"
    ".gitignore"
    "resource.h"
    #"LawnProject.rc"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SexyAppBase)

################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG            "PlantsVsZombies"
    TARGET_NAME_RELEASE          "PlantsVsZombies"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG            "${CMAKE_BINARY_DIR}/Distro"
    OUTPUT_DIRECTORY_RELEASE          "${CMAKE_BINARY_DIR}/Distro"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        #"_WINDOWS;"
        #"_VISUALC_;"
        #"_JPEGLIB_;"
        #"_CRT_SECURE_NO_WARNINGS"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "RELEASEFINAL;"
        #"_WINDOWS;"
        #"_VISUALC_;"
        #"_JPEGLIB_"
    ">"
    "_LIB;"
    "_MBCS"
)

################################################################################
# RC
################################################################################
set(RC_DIR ${CMAKE_SOURCE_DIR}/res)
set(RC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)

create_resources(${RC_DIR} ${RC_OUTPUT_DIR}/resources.cpp)
target_sources(${PROJECT_NAME} PRIVATE
    ${RC_OUTPUT_DIR}/resources.cpp
)

################################################################################
# Precompiled headers
################################################################################

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:Common.h>"
)

################################################################################
# Dependencies
################################################################################

# If on windows, link to the windows libraries, for SDL2 support
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "winmm.lib;"
            "ws2_32.lib;"
            "Imm32.lib;"
            "Cfgmgr32.lib;"
            "Version.lib;"
            "Setupapi.lib;"
            #"ddraw.lib;"
        ">"
        "$<$<CONFIG:Release>:"
            "winmm.lib;"
            "ws2_32.lib;"
            "Imm32.lib;"
            "Cfgmgr32.lib;"
            "Version.lib;"
            "Setupapi.lib;"
            #"ddraw.lib;"
        ">"
    )
endif()

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        #"ddraw;"
    ">"
    "$<$<CONFIG:Release>:"
	    #"ddraw;"
	#"ucrt;"
    ">"
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE ".") # SexyAppFramework
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
