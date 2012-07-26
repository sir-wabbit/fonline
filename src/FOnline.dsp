# Microsoft Developer Studio Project File - Name="FOnline" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FOnline - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FOnline.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FOnline.mak" CFG="FOnline - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FOnline - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FOnline - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FOnline - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"G:\FOnline\client_rel\FOnline.exe"

!ELSEIF  "$(CFG)" == "FOnline - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FOnline___Win32_Debug"
# PROP BASE Intermediate_Dir "FOnline___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"E:\Games\Fallout2\FOnline\FOnline.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"G:\FOnline\client\FOnline.exe"

!ENDIF 

# Begin Target

# Name "FOnline - Win32 Release"
# Name "FOnline - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BufMngr.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileMngr.cpp
# End Source File
# Begin Source File

SOURCE=.\CFont.cpp
# End Source File
# Begin Source File

SOURCE=.\CHexField.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\Critter.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpriteManager.cpp
# End Source File
# Begin Source File

SOURCE=.\datfile.cpp
# End Source File
# Begin Source File

SOURCE=.\FEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\FEngine_Iface.cpp
# End Source File
# Begin Source File

SOURCE=.\keyb.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundMngr.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BufMngr.h
# End Source File
# Begin Source File

SOURCE=.\CFileMngr.h
# End Source File
# Begin Source File

SOURCE=.\CFont.h
# End Source File
# Begin Source File

SOURCE=.\CHexField.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\Critter.h
# End Source File
# Begin Source File

SOURCE=.\CSpriteManager.h
# End Source File
# Begin Source File

SOURCE=.\datfile.h
# End Source File
# Begin Source File

SOURCE=.\FEngine.h
# End Source File
# Begin Source File

SOURCE=.\frmload.h
# End Source File
# Begin Source File

SOURCE=.\keyb.h
# End Source File
# Begin Source File

SOURCE=.\netproto.h
# End Source File
# Begin Source File

SOURCE=.\SoundMngr.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\fico.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\rs.rc
# End Source File
# End Group
# Begin Group "cfile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfile\cfile.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\cfile\cfile.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\gzio.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\maketree.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "unlzss"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\unlzss\unlzss.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\unlzss\unlzss.h
# End Source File
# End Group
# Begin Group "decomp_acm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\decomp_acm\acmstrm.cpp
# End Source File
# Begin Source File

SOURCE=.\decomp_acm\acmstrm.h
# End Source File
# End Group
# Begin Group "ogg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ogg\codec.h
# End Source File
# Begin Source File

SOURCE=.\ogg\ogg.h
# End Source File
# Begin Source File

SOURCE=.\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=.\ogg\vorbisenc.h
# End Source File
# Begin Source File

SOURCE=.\ogg\vorbisfile.h
# End Source File
# End Group
# End Target
# End Project
