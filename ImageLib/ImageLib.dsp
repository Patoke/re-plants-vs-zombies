# Microsoft Developer Studio Project File - Name="ImageLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ImageLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImageLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageLib.mak" CFG="ImageLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ImageLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WinAlch", YKBAAAAA"
# PROP Scc_LocalPath "..\winalch"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_WINDOWS" /D "_VISUALC_" /D "_IMAGE_MAGICK_JPEG_" /D "_JPEGLIB_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo"WinDM.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ImageLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_WINDOWS" /D "_VISUALC_" /D "_IMAGE_MAGICK_JPEG_" /D "_JPEGLIB_" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"WinDM.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ImageLib - Win32 Release"
# Name "ImageLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ImageLib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ImageLib.h
# End Source File
# End Group
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\png\png.c
# End Source File
# Begin Source File

SOURCE=.\png\png.h
# End Source File
# Begin Source File

SOURCE=.\png\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=.\png\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\png\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\png\pngget.c
# End Source File
# Begin Source File

SOURCE=.\png\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\png\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\png\pngread.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\png\pngset.c
# End Source File
# Begin Source File

SOURCE=.\png\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\png\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwutil.c
# End Source File
# End Group
# Begin Group "JPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jpeg\cderror.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\cdjpeg.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jutils.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\jversion.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdbmp.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdcolmap.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdgif.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdppm.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdrle.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdswitch.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\rdtarga.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\transupp.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\transupp.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\wrbmp.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\wrgif.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\wrppm.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\wrrle.c
# End Source File
# Begin Source File

SOURCE=.\jpeg\wrtarga.c
# End Source File
# End Group
# End Target
# End Project
