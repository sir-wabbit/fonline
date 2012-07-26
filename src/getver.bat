@echo off

set MAJORNO=1
set MINORNO=0
echo set BUILDNO=$WCREV$>wcrev.in
"E:\Program Files\Subversion\bin\SubWCRev.exe" . wcrev.in wcrev.bat > NUL
erase wcrev.in
call wcrev.bat
erase wcrev.bat

echo #ifndef __VERSION_H__ > version.h
echo #define __VERSION_H__ >> version.h
echo #define MAJORNO %MAJORNO% >> version.h
echo #define MINORNO %MINORNO% >> version.h
echo #define BUILDNO %BUILDNO% >> version.h
echo #define VERSTR "%MAJORNO%.%MINORNO%" >> version.h
echo #define BUILDSTR "%BUILDNO%" >> version.h
echo #define FULLVERSTR "%MAJORNO%.%MINORNO%.%BUILDNO%" >> version.h
echo #endif __VERSION_H__ >> version.h
