# Microsoft Developer Studio Project File - Name="SexyAppBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SexyAppBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SexyAppBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SexyAppBase.mak" CFG="SexyAppBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SexyAppBase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SexyAppBase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WinAlch", YKBAAAAA"
# PROP Scc_LocalPath "..\winalch"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SexyAppBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseVC6"
# PROP Intermediate_Dir "ReleaseVC6"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Oy- /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_WINDOWS" /D "_VISUALC_" /D "_JPEGLIB_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SexyAppBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugVC6"
# PROP Intermediate_Dir "DebugVC6"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_WINDOWS" /D "_VISUALC_" /D "_JPEGLIB_" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SexyAppBase - Win32 Release"
# Name "SexyAppBase - Win32 Debug"
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Group "Graphics Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Color.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\D3DInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\D3DTester.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DDImage.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DDInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Graphics.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphicsBuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ImageFont.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MemoryImage.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Quantize.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SWTri.cpp
# End Source File
# Begin Source File

SOURCE=.\SysFont.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Graphics Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\D3DInterface.h
# End Source File
# Begin Source File

SOURCE=.\D3DTester.h
# End Source File
# Begin Source File

SOURCE=.\DDImage.h
# End Source File
# Begin Source File

SOURCE=.\DDInterface.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\Graphics.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\ImageFont.h
# End Source File
# Begin Source File

SOURCE=.\MemoryImage.h
# End Source File
# Begin Source File

SOURCE=.\Quantize.h
# End Source File
# Begin Source File

SOURCE=.\SWTri.h
# End Source File
# Begin Source File

SOURCE=.\SysFont.h
# End Source File
# End Group
# End Group
# Begin Group "ImageLib"

# PROP Default_Filter ""
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImageLib\png\png.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\png.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngget.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngread.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngset.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\png\pngwutil.c
# End Source File
# End Group
# Begin Group "JPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImageLib\jpeg\cderror.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\cdjpeg.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jutils.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\jversion.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdbmp.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdcolmap.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdgif.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdppm.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdrle.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdswitch.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\rdtarga.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\transupp.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\transupp.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\wrbmp.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\wrgif.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\wrppm.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\wrrle.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\jpeg\wrtarga.c
# End Source File
# End Group
# Begin Group "ZLIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImageLib\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=..\ImageLib\zlib\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\ImageLib\ImageLib.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageLib\ImageLib.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Group "Misc Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Buffer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CritSect.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Debug.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DescParser.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DirectXErrorString.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Flags.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\HTTPTransfer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\KeyCodes.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MiscBuildBase.cpp
# End Source File
# Begin Source File

SOURCE=.\MTRand.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\PerfTimer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\PropertiesParser.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SEHCatcher.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SexyMatrix.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\XMLParser.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Misc Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoCrit.h
# End Source File
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\CritSect.h
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\DescParser.h
# End Source File
# Begin Source File

SOURCE=.\DirectXErrorString.h
# End Source File
# Begin Source File

SOURCE=.\Flags.h
# End Source File
# Begin Source File

SOURCE=.\HTTPTransfer.h
# End Source File
# Begin Source File

SOURCE=.\KeyCodes.h
# End Source File
# Begin Source File

SOURCE=.\MTRand.h
# End Source File
# Begin Source File

SOURCE=.\PerfTimer.h
# End Source File
# Begin Source File

SOURCE=.\Point.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesParser.h
# End Source File
# Begin Source File

SOURCE=.\Rect.h
# End Source File
# Begin Source File

SOURCE=.\SEHCatcher.h
# End Source File
# Begin Source File

SOURCE=.\SexyMatrix.h
# End Source File
# Begin Source File

SOURCE=.\SexyVector.h
# End Source File
# Begin Source File

SOURCE=.\XMLParser.h
# End Source File
# End Group
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Group "Sound Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BassLoader.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BassMusicInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DSoundInstance.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DSoundManager.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FModLoader.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FModMusicInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FModSoundInstance.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FModSoundManager.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MusicInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SoundBuild.cpp
# End Source File
# End Group
# Begin Group "Sound Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BassLoader.h
# End Source File
# Begin Source File

SOURCE=.\BassMusicInterface.h
# End Source File
# Begin Source File

SOURCE=.\DSoundInstance.h
# End Source File
# Begin Source File

SOURCE=.\DSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\dsoundversion.h
# End Source File
# Begin Source File

SOURCE=.\FModLoader.h
# End Source File
# Begin Source File

SOURCE=.\FModMusicInterface.h
# End Source File
# Begin Source File

SOURCE=.\FModSoundInstance.h
# End Source File
# Begin Source File

SOURCE=.\FModSoundManager.h
# End Source File
# Begin Source File

SOURCE=.\MusicInterface.h
# End Source File
# Begin Source File

SOURCE=.\SoundInstance.h
# End Source File
# Begin Source File

SOURCE=.\SoundManager.h
# End Source File
# End Group
# Begin Group "ogg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ogg\asm_arm.h
# End Source File
# Begin Source File

SOURCE=.\ogg\backends.h
# End Source File
# Begin Source File

SOURCE=.\ogg\bitwise.c
# End Source File
# Begin Source File

SOURCE=.\ogg\block.c
# End Source File
# Begin Source File

SOURCE=.\ogg\codebook.c
# End Source File
# Begin Source File

SOURCE=.\ogg\codebook.h
# End Source File
# Begin Source File

SOURCE=.\ogg\codec_internal.h
# End Source File
# Begin Source File

SOURCE=.\ogg\config_types.h
# End Source File
# Begin Source File

SOURCE=.\ogg\floor0.c
# End Source File
# Begin Source File

SOURCE=.\ogg\floor1.c
# End Source File
# Begin Source File

SOURCE=.\ogg\framing.c
# End Source File
# Begin Source File

SOURCE=.\ogg\info.c
# End Source File
# Begin Source File

SOURCE=.\ogg\ivorbiscodec.h
# End Source File
# Begin Source File

SOURCE=.\ogg\ivorbisfile.h
# End Source File
# Begin Source File

SOURCE=.\ogg\lsp_lookup.h
# End Source File
# Begin Source File

SOURCE=.\ogg\mapping0.c
# End Source File
# Begin Source File

SOURCE=.\ogg\mdct.c
# End Source File
# Begin Source File

SOURCE=.\ogg\mdct.h
# End Source File
# Begin Source File

SOURCE=.\ogg\mdct_lookup.h
# End Source File
# Begin Source File

SOURCE=.\ogg\misc.h
# End Source File
# Begin Source File

SOURCE=.\ogg\ogg.h
# End Source File
# Begin Source File

SOURCE=.\ogg\os.h
# End Source File
# Begin Source File

SOURCE=.\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=.\ogg\registry.c
# End Source File
# Begin Source File

SOURCE=.\ogg\registry.h
# End Source File
# Begin Source File

SOURCE=.\ogg\res012.c
# End Source File
# Begin Source File

SOURCE=.\ogg\sharedbook.c
# End Source File
# Begin Source File

SOURCE=.\ogg\synthesis.c
# End Source File
# Begin Source File

SOURCE=.\ogg\vorbisfile.c
# End Source File
# Begin Source File

SOURCE=.\ogg\window.c
# End Source File
# Begin Source File

SOURCE=.\ogg\window.h
# End Source File
# Begin Source File

SOURCE=.\ogg\window_lookup.h
# End Source File
# End Group
# End Group
# Begin Group "Widget"

# PROP Default_Filter ""
# Begin Group "Widget Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ButtonWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Checkbox.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Dialog.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DialogButton.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\EditWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\HyperlinkWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Insets.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ListWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ScrollbarWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ScrollbuttonWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Slider.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\TextWidget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Widget.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\WidgetBuildBase.cpp
# End Source File
# Begin Source File

SOURCE=.\WidgetContainer.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\WidgetManager.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Widget Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ButtonListener.h
# End Source File
# Begin Source File

SOURCE=.\ButtonWidget.h
# End Source File
# Begin Source File

SOURCE=.\Checkbox.h
# End Source File
# Begin Source File

SOURCE=.\CheckboxListener.h
# End Source File
# Begin Source File

SOURCE=.\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\DialogButton.h
# End Source File
# Begin Source File

SOURCE=.\DialogListener.h
# End Source File
# Begin Source File

SOURCE=.\EditListener.h
# End Source File
# Begin Source File

SOURCE=.\EditWidget.h
# End Source File
# Begin Source File

SOURCE=.\HyperlinkWidget.h
# End Source File
# Begin Source File

SOURCE=.\Insets.h
# End Source File
# Begin Source File

SOURCE=.\ListListener.h
# End Source File
# Begin Source File

SOURCE=.\ListWidget.h
# End Source File
# Begin Source File

SOURCE=.\ScrollbarWidget.h
# End Source File
# Begin Source File

SOURCE=.\ScrollbuttonWidget.h
# End Source File
# Begin Source File

SOURCE=.\ScrollListener.h
# End Source File
# Begin Source File

SOURCE=.\Slider.h
# End Source File
# Begin Source File

SOURCE=.\SliderListener.h
# End Source File
# Begin Source File

SOURCE=.\TextWidget.h
# End Source File
# Begin Source File

SOURCE=.\Widget.h
# End Source File
# Begin Source File

SOURCE=.\WidgetContainer.h
# End Source File
# Begin Source File

SOURCE=.\WidgetManager.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\SexyAppBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SexyAppBase.h
# End Source File
# End Target
# End Project
