@echo off
setlocal
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cl main.c /I"include" /I"include/CL" /MD /O2 /Fe:sdl3_app.exe /link SDL3.lib /SUBSYSTEM:CONSOLE
if %errorlevel% == 0 (
    echo Компиляция успешна!
    sdl3_app.exe
) else (
    echo Ошибка компиляции!
    pause
)
endlocal