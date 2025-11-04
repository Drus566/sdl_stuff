
@REM @echo off
@REM setlocal
@REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@REM cl main.c /I"include" /MD /O2 /Fe:sdl3_app.exe /link SDL3.lib /SUBSYSTEM:CONSOLE
@REM if %errorlevel% == 0 (
@REM     echo Компиляция успешна!
@REM     sdl3_app.exe
@REM ) else (
@REM     echo Ошибка компиляции!
@REM     pause
@REM )
@REM endlocal


@echo off
setlocal EnableDelayedExpansion

echo ================================
echo Simple SDL3 Project Builder
echo ================================

set SOURCE_DIR=src
set OUTPUT_DIR=build
set EXE_NAME=simple_sdl3.exe

set SDL3_INCLUDE=libs\sdl3\include
set SDL3_LIB=libs\sdl3\x64

if not exist "%SOURCE_DIR%\main.c" (
    echo Error: Source file not found - %SOURCE_DIR%\main.c
    pause
    exit /b 1
)

echo Initializing Visual Studio environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

if not exist "%OUTPUT_DIR%" (
    echo Creating output directory...
    mkdir "%OUTPUT_DIR%"
)

echo Compiling %EXE_NAME%...
cl /nologo /Zi /W4 /Fe:%OUTPUT_DIR%\%EXE_NAME% ^
    /I"%SDL3_INCLUDE%" ^
    %SOURCE_DIR%\main.c ^
    /link /LIBPATH:"%SDL3_LIB%" SDL3.lib

if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Build successfull: %OUTPUT_DIR%\%EXE_NAME%
echo.

if exist "%SDL3_LIB%\SDL3.dll" (
    copy "%SDL3_LIB%\SDL3.dll" "%OUTPUT_DIR%\SDL3.dll" > nul
    echo Copied SDL3.dll to output directory
)

echo ===============================
echo Ready to run! Execute: run.bat
echo ===============================

endlocal