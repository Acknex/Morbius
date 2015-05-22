@echo off
rem adjust path to engine
Set "AckPath=d:\acknex8" 

Set "path=%path%;%AckPath%" 

echo.
echo Generate Bindings...
acknex create_bindings.c

echo.
echo Create Resources and Executable...

rem options.scr does not allow to ignore certain folders so provoke 
rem file not found for everything in "media" with folder renaming

ren media media_renamed_by_buildscript
wed -r main.c
rem wed -p main.c
ren media_renamed_by_buildscript media

echo.
echo Copy unresourced files...
xcopy /y media\*.* main.cd\media\
xcopy  main.cd\*.xml main.cd\xml\
del main.cd\*.xml
del main.exe

echo.
echo Finished.
echo.

pause