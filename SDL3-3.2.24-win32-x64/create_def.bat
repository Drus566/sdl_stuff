@echo off
echo Создание SDL3.def из вывода dumpbin...

(
echo EXPORTS
for /f "skip=19 tokens=4" %%a in ('dumpbin /exports SDL3.dll') do (
  if not "%%a"=="" echo %%a
)
) > SDL3.def

echo Файл SDL3.def создан успешно!