@echo off

set FILETOZIP=%1
echo Selected file: %FILETOZIP%

set TEMPDIR=TMP
echo Creating temporary file directory .\%TEMPDIR%
mkdir %TEMPDIR%

rem copy %FILETOZIP% %TEMPDIR%\main.c
gcc -E %FILETOZIP% -Dinclude=#include -Isystem-headers -o%TEMPDIR%\main_tmp.c
grep -v '^# [0-9]' %TEMPDIR%\main_tmp.c > %TEMPDIR%\main.c
erase %TEMPDIR%\main_tmp.c
rem copy *.h %TEMPDIR%



cd %TEMPDIR%    
rem echo Starting 7zip with arguments: %ProgramFiles%\7-Zip\7z a -y -tzip main.zip main.c
"%ProgramFiles%\7-Zip\7z" a -y -tzip main.zip *

cd ..
xcopy %TEMPDIR%\main.zip . /Y /I
pause
echo Removing the temporary directory
rmdir /S /Q %TEMPDIR%
IF %2=="" (
  pause
)