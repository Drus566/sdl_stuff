@echo off
setlocal EnableDelayedExpansion

echo ========================================
echo Creating .lib from .dll
echo ========================================

if "%~1"=="" (
  echo Usage: %~nx0 "path\to\file.dll" [architecture]
  echo Example: %~nx0 "SDL3.dll" x64
  echo Example: %~nx0 "SDL3.dll" x86
  echo.
  echo Default architecture: x64
  pause
  exit /b 1
)

set "DLL_PATH=%~1"
set "DLL_DIR=%~dp1"
set "DLL_NAME=%~n1"

set "ARCH=%~2"
if "!ARCH!"=="" set "ARCH=x64"

if not exist "%DLL_PATH%" (
  echo Error: File not found - %DLL_PATH%
  pause
  exit /b 1
)

echo Input DLL: %DLL_PATH%
echo Output name: %DLL_NAME%
echo Architecture: %ARCH%

pushd "%DLL_DIR%"

echo Step 1: Get exports from DLL...
dumpbin /exports "%DLL_NAME%.dll" > exports.tmp

echo Step 2: Create .def file...
(
  echo EXPORTS
  for /f "skip=19 tokens=4" %%a in (exports.tmp) do (
    if not "%%a"=="" echo %%a
  )
) > "%DLL_NAME%.def"

echo Step 3: Create .lib file...
lib /def:"%DLL_NAME%.def" /out:"%DLL_NAME%.lib" /machine:!ARCH!

echo Step 4: Clean up...
del exports.tmp

popd

echo ========================================
echo Done!
echo Created files in: %DLL_DIR%
echo  - %DLL_NAME%.def
echo  - %DLL_NAME%.lib
echo  - %DLL_NAME%.exp
echo ========================================

pause