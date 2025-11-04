@echo off
setlocal

REM Требуется: SDL3_INC = путь к папке include (SDL3), SDL3_LIB = путь к папке lib (SDL3)
if "%SDL3_INC%"=="" (
    echo Установите переменную окружения SDL3_INC (путь к SDL3 include), например:
    echo    set SDL3_INC=C:\path\to\SDL3\include
)
if "%SDL3_LIB%"=="" (
    echo Установите переменную окружения SDL3_LIB (путь к SDL3 lib), например:
    echo    set SDL3_LIB=C:\path\to\SDL3\lib
)
if "%SDL3_INC%"=="" goto :BADENV
if "%SDL3_LIB%"=="" goto :BADENV

if not exist build (
    mkdir build
)

where cl >nul 2>&1
if %ERRORLEVEL%==0 goto :BUILD_MSVC

where gcc >nul 2>&1
if %ERRORLEVEL%==0 goto :BUILD_GCC

echo Ошибка: не найден компилятор cl (MSVC) или gcc (MinGW). Откройте соответствующий shell.
goto :EOF

:BADENV
echo Ошибка: SDL3_INC или SDL3_LIB не заданы.
goto :EOF

:BUILD_MSVC
echo Using MSVC (cl)...
REM Компиляция с MSVC. Требуется Developer Command Prompt.
cl /nologo /Ox /MD /I "include" /I "%SDL3_INC%" src\*.c "%SDL3_LIB%\SDL3.lib" "%SDL3_LIB%\SDL3main.lib" /link /OUT:build\t3st.exe
if %ERRORLEVEL% neq 0 (
    echo Сборка MSVC провалена.
    goto :EOF
)
echo Сборка MSVC успешна -> build\t3st.exe
goto :EOF

:BUILD_GCC
echo Using GCC (MinGW)...
REM Компиляция с MinGW (gcc). Убедитесь, что SDL3 libs для MinGW доступны в SDL3_LIB.
gcc -O2 -I"include" -I"%SDL3_INC%" src\*.c -L"%SDL3_LIB%" -lSDL3 -lSDL3main -lwinmm -lgdi32 -o build\t3st.exe
if %ERRORLEVEL% neq 0 (
    echo Сборка GCC провалена.
    goto :EOF
)
echo Сборка GCC успешна -> build\t3st.exe
goto :EOF