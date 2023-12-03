set(PROJECT_NAME LawnProject)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../Cursor13.cur"
    "../LawnApp.cpp"
    "../LawnApp.h"
    "../main.cpp"
    ".gitattributes"
    ".gitignore"
    "resource.h"
    "LawnProject.rc"
)
source_group("" FILES ${no_group_source_files})

set(ImageLib
    "ImageLib.cpp"
    "ImageLib.h"
)
source_group("ImageLib" FILES ${ImageLib})

set(ImageLib__JPEG_SOURCE_DIR ${CMAKE_BINARY_DIR}/libjpeg)
set(ImageLib__JPEG_LIB_DIR ${CMAKE_BINARY_DIR}/libjpeg-build)

include(ExternalProject)
ExternalProject_Add(libjpeg-src
    GIT_REPOSITORY      https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG             3.0.1
    SOURCE_DIR          "${ImageLib__JPEG_SOURCE_DIR}"
    BINARY_DIR          "${ImageLib__JPEG_LIB_DIR}"
    CMAKE_ARGS          "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                        "-DCMAKE_INSTALL_PREFIX=${ImageLib__JPEG_LIB_DIR}/install" # unused, have to specify
                        "-DCMAKE_C_FLAGS=${universal_cflags}"
                        "-DENABLE_SHARED=FALSE"
                        "-DWITH_TURBOJPEG=FALSE"
                        "-Wno-dev"

    INSTALL_COMMAND     ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/jpeg/;
    COMMAND             ${CMAKE_COMMAND} -E copy ${ImageLib__JPEG_SOURCE_DIR}/jpeglib.h
                                                 ${ImageLib__JPEG_SOURCE_DIR}/jerror.h
                                                 ${ImageLib__JPEG_LIB_DIR}/jconfig.h
                                                 ${ImageLib__JPEG_SOURCE_DIR}/jmorecfg.h
                                                 ${ImageLib__JPEG_SOURCE_DIR}/jpegint.h
                                            ${CMAKE_BINARY_DIR}/include/jpeg/

    BUILD_BYPRODUCTS    "${ImageLib__JPEG_LIB_DIR}/libjpeg.a"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jpeglib.h"
                        "${CMAKE_BINARY_DIR}/include/jpeg/jerror.h"
    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_UPDATE TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD TRUE
    UPDATE_DISCONNECTED TRUE
)

add_library(jpeg STATIC IMPORTED)
add_dependencies(jpeg libjpeg-src)
set_target_properties(jpeg
    PROPERTIES IMPORTED_LOCATION ${ImageLib__JPEG_LIB_DIR}/libjpeg.a
)

set(ImageLib__JPEG_H
    "${CMAKE_BINARY_DIR}/include/jpeg/jpeglib.h"
    "${CMAKE_BINARY_DIR}/include/jpeg/jerror.h"
)
source_group("ImageLib\\JPEG" FILES ${ImageLib__JPEG_H})

set(ImageLib__ZLIB_SOURCE_DIR ${CMAKE_BINARY_DIR}/zlib)
set(ImageLib__ZLIB_LIB_DIR ${CMAKE_BINARY_DIR}/zlib-build)
ExternalProject_Add(zlib-src
    GIT_REPOSITORY          https://github.com/madler/zlib.git
    GIT_TAG                 v1.3
    SOURCE_DIR              "${ImageLib__ZLIB_SOURCE_DIR}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${ImageLib__ZLIB_LIB_DIR}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-Wno-dev"
    BUILD_COMMAND           "${CMAKE_COMMAND}"
                            "--build" "${ImageLib__ZLIB_LIB_DIR}"
                            "--target" "zlibstatic"
    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/zlib/;
    COMMAND                 ${CMAKE_COMMAND} -E copy ${ImageLib__ZLIB_SOURCE_DIR}/zlib.h
                                                     ${ImageLib__ZLIB_LIB_DIR}/zconf.h
                                                ${CMAKE_BINARY_DIR}/include/zlib/

    BINARY_DIR              ${ImageLib__ZLIB_LIB_DIR}
    BUILD_BYPRODUCTS        "${ImageLib__ZLIB_LIB_DIR}/libzlibstatic.a"
                            "${CMAKE_BINARY_DIR}/include/zlib/zlib.h" # Use a custom dir to avoid system headers
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(zlib STATIC IMPORTED)
add_dependencies(zlib zlib-src)
set_target_properties(zlib
    PROPERTIES IMPORTED_LOCATION ${ImageLib__ZLIB_LIB_DIR}/libzlibstatic.a
)

set(ImageLib__ZLIB_H
    "${CMAKE_BINARY_DIR}/include/zlib/zlib.h"
)
source_group("ImageLib\\ZLIB" FILES ${ImageLib__ZLIB_H})

# =========================================================

set(ImageLib__PNG_SOURCE_DIR ${CMAKE_BINARY_DIR}/libpng)
set(ImageLib__PNG_LIB_DIR ${CMAKE_BINARY_DIR}/libpng-build)
ExternalProject_Add(libpng-src
    GIT_REPOSITORY          https://github.com/glennrp/libpng.git
    GIT_TAG                 v1.6.40
    SOURCE_DIR              "${ImageLib__PNG_SOURCE_DIR}"
    CMAKE_ARGS              "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}"
                            "-DCMAKE_INSTALL_PREFIX=${ImageLib__PNG_LIB_DIR}"
                            "-DCMAKE_C_FLAGS=${universal_cflags}"
                            "-DPNG_SHARED=OFF"
                            "-DPNG_EXECUTABLES=OFF"
                            "-DPNG_TESTS=OFF"
                            "-Wno-dev"

    INSTALL_COMMAND         ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include/png/
    COMMAND                 ${CMAKE_COMMAND} -E copy ${ImageLib__PNG_SOURCE_DIR}/png.h
                                                     ${ImageLib__PNG_SOURCE_DIR}/pngconf.h
                                                     ${ImageLib__PNG_LIB_DIR}/pnglibconf.h
                                                     ${CMAKE_BINARY_DIR}/include/png/                                            
    BINARY_DIR              ${ImageLib__PNG_LIB_DIR}
    BUILD_BYPRODUCTS        "${ImageLib__PNG_LIB_DIR}/libpng.a"
                            "${CMAKE_BINARY_DIR}/include/png/png.h" # Use a custom dir to avoid system headers
    USES_TERMINAL_DOWNLOAD  TRUE
    USES_TERMINAL_UPDATE    TRUE
    USES_TERMINAL_CONFIGURE TRUE
    USES_TERMINAL_BUILD     TRUE
    USES_TERMINAL_INSTALL   TRUE
    UPDATE_DISCONNECTED     TRUE
)

add_library(libpng STATIC IMPORTED)
add_dependencies(libpng libpng-src)
set_target_properties(libpng
    PROPERTIES IMPORTED_LOCATION ${ImageLib__PNG_LIB_DIR}/libpng.a
)

# =========================================================

set(ImageLib__PNG_H
    "${CMAKE_BINARY_DIR}/include/png/png.h"
)
source_group("ImageLib\\PNG" FILES ${ImageLib__PNG_H})

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
    "../Lawn/System/PopDRMComm.h"
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
    "../Lawn/System/PopDRMComm.cpp"
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

set(PakLib
    "../PakLib/PakInterface.cpp"
    "../PakLib/PakInterface.h"
)
source_group("PakLib" FILES ${PakLib})

set(Sexy
    "SexyApp.cpp"
    "SexyApp.h"
    "SexyAppBase.cpp"
    "SexyAppBase.h"
)
source_group("Sexy" FILES ${Sexy})

set(Sexy__Graphics__Graphics_Include
    "Color.h"
    "D3DInterface.h"
    "D3DTester.h"
    "DDImage.h"
    "DDInterface.h"
    "Font.h"
    "Graphics.h"
    "Image.h"
    "ImageFont.h"
    "MemoryImage.h"
    "NativeDisplay.h"
    "Quantize.h"
    "SWTri.h"
    "SysFont.h"
)
source_group("Sexy\\Graphics\\Graphics Include" FILES ${Sexy__Graphics__Graphics_Include})

set(Sexy__Graphics__Graphics_Source
#    "Color.cpp"
#    "D3DInterface.cpp"
#    "D3DTester.cpp"
#    "DDImage.cpp"
#    "DDInterface.cpp"
#    "Font.cpp"
#    "Graphics.cpp"
    "GraphicsBuild.cpp"
#    "Image.cpp"
#    "ImageFont.cpp"
#    "MemoryImage.cpp"
#    "NativeDisplay.cpp"
#    "Quantize.cpp"
    "SWTri.cpp"
#    "SysFont.cpp"
)
source_group("Sexy\\Graphics\\Graphics Source" FILES ${Sexy__Graphics__Graphics_Source})

set(Sexy__Graphics__Inc_Routines
    "BltRotatedHelper.inc"
    "DDI_Additive.inc"
    "DDI_AlphaBlt.inc"
    "DDI_BltRotated.inc"
    "DDI_BltRotated_Additive.inc"
    "DDI_FastBlt_NoAlpha.inc"
    "DDI_FastStretch.inc"
    "DDI_FastStretch_Additive.inc"
    "DDI_NormalBlt_Volatile.inc"
    "MI_AdditiveBlt.inc"
    "MI_BltRotated.inc"
    "MI_BltRotated_Additive.inc"
    "MI_GetNativeAlphaData.inc"
    "MI_GetRLAlphaData.inc"
    "MI_NormalBlt.inc"
    "MI_SlowStretchBlt.inc"
)
source_group("Sexy\\Graphics\\Inc Routines" FILES ${Sexy__Graphics__Inc_Routines})

set(Sexy__Misc__Misc_Include
    "AutoCrit.h"
    "Buffer.h"
    "Common.h"
    "CritSect.h"
    "Debug.h"
    "DirectXErrorString.h"
    "Flags.h"
    "HTTPTransfer.h"
    "KeyCodes.h"
    "memmgr.h"
    "ModVal.h"
    "MTRand.h"
    "PerfTimer.h"
    "Point.h"
    "PropertiesParser.h"
    "Rect.h"
    "ResourceManager.h"
    "SEHCatcher.h"
    "SexyMatrix.h"
    "SexyVector.h"
    "SmartPtr.h"
    "XMLParser.h"
)
source_group("Sexy\\Misc\\Misc Include" FILES ${Sexy__Misc__Misc_Include})

set(Sexy__Misc__Misc_Source
#    "Buffer.cpp"
#    "Common.cpp"
#    "CritSect.cpp"
#    "Debug.cpp"
#    "DirectXErrorString.cpp"
#    "Flags.cpp"
#    "HTTPTransfer.cpp"
#    "KeyCodes.cpp"
    "MiscBuildBase.cpp"
#    "ModVal.cpp"
#    "MTRand.cpp"
#    "PerfTimer.cpp"
#    "PropertiesParser.cpp"
#    "ResourceManager.cpp"
#    "SEHCatcher.cpp"
#    "SexyMatrix.cpp"
#    "XMLParser.cpp"
)
source_group("Sexy\\Misc\\Misc Source" FILES ${Sexy__Misc__Misc_Source})

set(Sexy__Sound__Ogg
    "ogg/asm_arm.h"
    "ogg/backends.h"
    "ogg/bitwise.c"
    "ogg/block.c"
    "ogg/codebook.c"
    "ogg/codebook.h"
    "ogg/codec_internal.h"
    "ogg/config_types.h"
    "ogg/floor0.c"
    "ogg/floor1.c"
    "ogg/framing.c"
    "ogg/info.c"
    "ogg/ivorbiscodec.h"
    "ogg/ivorbisfile.h"
    "ogg/lsp_lookup.h"
    "ogg/mapping0.c"
    "ogg/mdct.c"
    "ogg/mdct.h"
    "ogg/mdct_lookup.h"
    "ogg/misc.h"
    "ogg/ogg.h"
    "ogg/os.h"
    "ogg/os_types.h"
    "ogg/registry.c"
    "ogg/registry.h"
    "ogg/res012.c"
    "ogg/sharedbook.c"
    "ogg/synthesis.c"
    "ogg/vorbisfile.c"
    "ogg/window.c"
    "ogg/window.h"
    "ogg/window_lookup.h"
)
source_group("Sexy\\Sound\\Ogg" FILES ${Sexy__Sound__Ogg})

set(Sexy__Sound__Sound_Include
    "BassMusicInterface.h"
    "DSoundInstance.h"
    "DSoundManager.h"
    "FModLoader.h"
    "FModMusicInterface.h"
    "FModSoundInstance.h"
    "FModSoundManager.h"
    "MusicInterface.h"
    "SoundInstance.h"
    "SoundManager.h"
)
source_group("Sexy\\Sound\\Sound Include" FILES ${Sexy__Sound__Sound_Include})

set(Sexy__Sound__Sound_Source
#    "BassLoader.cpp"
    "BassLoader.h"
#    "BassMusicInterface.cpp"
#    "DSoundInstance.cpp"
#    "DSoundManager.cpp"
#    "FModLoader.cpp"
#    "FModMusicInterface.cpp"
#    "FModSoundInstance.cpp"
#    "FModSoundManager.cpp"
#    "Insets.cpp"
#    "MusicInterface.cpp"
    "SoundBuild.cpp"
)
source_group("Sexy\\Sound\\Sound Source" FILES ${Sexy__Sound__Sound_Source})

set(Sexy__Widget__Widget_Include
    "ButtonListener.h"
    "ButtonWidget.h"
    "Checkbox.h"
    "CheckboxListener.h"
    "Dialog.h"
    "DialogButton.h"
    "DialogListener.h"
    "EditListener.h"
    "EditWidget.h"
    "FlashWidget.h"
    "HyperlinkWidget.h"
    "Insets.h"
    "ListListener.h"
    "ListWidget.h"
    "ScrollbarWidget.h"
    "ScrollbuttonWidget.h"
    "ScrollListener.h"
    "Slider.h"
    "SliderListener.h"
    "TextWidget.h"
    "Widget.h"
    "WidgetContainer.h"
    "WidgetManager.h"
)
source_group("Sexy\\Widget\\Widget Include" FILES ${Sexy__Widget__Widget_Include})

set(Sexy__Widget__Widget_Source
#    "ButtonWidget.cpp"
#    "Checkbox.cpp"
#    "Dialog.cpp"
#    "DialogButton.cpp"
#    "EditWidget.cpp"
#    "FlashWidget.cpp"
#    "HyperlinkWidget.cpp"
#    "ListWidget.cpp"
#    "ScrollbarWidget.cpp"
#    "ScrollbuttonWidget.cpp"
#    "Slider.cpp"
#    "TextWidget.cpp"
#    "Widget.cpp"
    "WidgetBuildBase.cpp"
#    "WidgetContainer.cpp"
#    "WidgetManager.cpp"
)
source_group("Sexy\\Widget\\Widget Source" FILES ${Sexy__Widget__Widget_Source})

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
    ${ImageLib}
#   ${ImageLib__JPEG2000}
    ${ImageLib__PNG_H}
    ${ImageLib__ZLIB_H}
    ${Lawn__Board__Board_Include}
    ${Lawn__Board__Board_Source}
    ${Lawn__Common__Common_Include}
    ${Lawn__Common__Common_Source}
    ${Lawn__System__System_Include}
    ${Lawn__System__System_Source}
    ${Lawn__Widget__Widget_Include}
    ${Lawn__Widget__Widget_Source}
    ${PakLib}
    ${Sexy}
    ${Sexy__Graphics__Graphics_Include}
    ${Sexy__Graphics__Graphics_Source}
    ${Sexy__Graphics__Inc_Routines}
    ${Sexy__Misc__Misc_Include}
    ${Sexy__Misc__Misc_Source}
    ${Sexy__Sound__Ogg}
    ${Sexy__Sound__Sound_Include}
    ${Sexy__Sound__Sound_Source}
    ${Sexy__Widget__Widget_Include}
    ${Sexy__Widget__Widget_Source}
    ${TodLib__Common__TodCommon_Include}
    ${TodLib__Common__TodCommon_Source}
    ${TodLib__Effect__Effect_Include}
    ${TodLib__Effect__Effect_Source}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SexyAppBase)

################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUGINCREMENTAL "SexyAppFramework"
    TARGET_NAME_DEBUGZYLOM       "SexyAppFramework"
    TARGET_NAME_DEBUG            "SexyAppFramework"
    TARGET_NAME_RELEASEZYLOM     "SexyAppFramework"
    TARGET_NAME_RELEASENOOP      "SexyAppFramework"
    TARGET_NAME_RELEASE          "SexyAppFramework"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGINCREMENTAL "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
    OUTPUT_DIRECTORY_DEBUGZYLOM       "${CMAKE_CURRENT_SOURCE_DIR}/./Debug - Zylom"
    OUTPUT_DIRECTORY_DEBUG            "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
    OUTPUT_DIRECTORY_RELEASEZYLOM     "${CMAKE_CURRENT_SOURCE_DIR}/./Release - Zylom"
    OUTPUT_DIRECTORY_RELEASENOOP      "${CMAKE_CURRENT_SOURCE_DIR}/./Release"
    OUTPUT_DIRECTORY_RELEASE          "${CMAKE_CURRENT_SOURCE_DIR}/./Release"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:DebugIncremental>:
        MultiThreadedDebug
    >
    $<$<CONFIG:DebugZylom>:
        MultiThreadedDebug
    >
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:ReleaseZylom>:
        MultiThreaded
    >
    $<$<CONFIG:Releasenoop>:
        MultiThreaded
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:DebugIncremental>,$<CONFIG:DebugZylom>,$<CONFIG:Debug>,$<CONFIG:ReleaseZylom>,$<CONFIG:Releasenoop>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugIncremental>:"
        "_DEBUG;"
        "_WINDOWS;"
        "_VISUALC_;"
        "_JPEGLIB_;"
        "_CRT_SECURE_NO_WARNINGS"
    ">"
    "$<$<CONFIG:DebugZylom>:"
        "_DEBUG;"
        "ZYLOM"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_WINDOWS;"
        "_VISUALC_;"
        "_JPEGLIB_;"
        "_CRT_SECURE_NO_WARNINGS"
    ">"
    "$<$<CONFIG:ReleaseZylom>:"
        "NDEBUG;"
        "ZYLOM"
    ">"
    "$<$<CONFIG:Releasenoop>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_WINDOWS;"
        "_VISUALC_;"
        "_JPEGLIB_"
    ">"
    "WIN32;"
    "_LIB;"
    "_MBCS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugIncremental>:
            /Od;
            /RTC1;
            /Zi
        >
        $<$<CONFIG:DebugZylom>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:Debug>:
            /Od;
            /Ob0;
            /Zi;
            /wd4996
        >
        $<$<CONFIG:ReleaseZylom>:
            /Ob1;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        $<$<CONFIG:Releasenoop>:
            /Ob1;
            /GF;
            /Gy;
            /Zi;
            /Oy-
        >
        $<$<CONFIG:Release>:
            /GL;
            /GF;
            /Gy;
            /Zi;
            /Oy-;
            /wd4996
        >
        /W3;
        /GR;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugIncremental>:
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /LTCG
        >
        /NOLOGO
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "winmm;"
        "ws2_32;"
        "ddraw;"
        "jpeg;"
        "zlib;"
        "libpng;"
    ">"
    "$<$<CONFIG:Release>:"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "winmm;"
        "ws2_32;"
	    "ddraw;"
        "jpeg;"
        "zlib;"
        "libpng;"
	#"ucrt;"
    ">"
)

#${ImageLib__JPEG_LIB_DIR}/libjpeg.a
#add_dependencies(libjpeg ImageLib__JPEG)
#add_dependencies(${PROJECT_NAME} ImageLib__JPEG)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++ "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
