@echo off
REM adjust path to engine
Set "AckPath=d:\acknex8" 

Set "path=%path%;%AckPath%" 

echo.
echo Generate Bindings...
acknex create_bindings.c

echo.
echo Create Resources and Executable...
WED -r main.c
REM WED -p main.c

echo.
echo Copy unresourced files...
xcopy /y media\*.* main.cd\media\
xcopy /y main.cd\*.xml main.cd\xml\
del main.cd\*.xml
del main.exe

echo.
echo Finished.
echo.

pause