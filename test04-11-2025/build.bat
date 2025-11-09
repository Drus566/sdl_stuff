@echo off
setlocal
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cl src\main.c /I"include" /MD /O2 /Fe:build\sdl3_app.exe /link /LIBPATH:"libs\sdl3\x64" SDL3.lib /SUBSYSTEM:CONSOLE

if %errorlevel% == 0 (
    echo Success compile! 
    del *.obj *.pdb *.ilk 2>nul
) else (
    echo Error compile!
    pause
)

if not exist "build\SDL3.dll" (
    copy "libs\sdl3\x64\SDL3.dll" "build\" >nul
)

endlocal