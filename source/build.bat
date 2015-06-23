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
timeout /t 1 /nobreak > NUL
wed -r morbius.c
rem wed -p morbius.c
ren media_renamed_by_buildscript media

echo.
echo Copy unresourced files...
xcopy /y media\*.* morbius.cd\media\
xcopy  morbius.cd\*.xml morbius.cd\xml\
copy wdf\acknex.wdf morbius.cd\acknex.wdf
del morbius.cd\*.xml
del morbius.exe

echo.
echo Finished.
echo.

pause