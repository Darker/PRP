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

FOR %%i IN ("%FILETOZIP%") DO (
set TEST_DIR=%%~ni
)
set TEST_DIR=TEST-%TEST_DIR%
echo %TEST_DIR%

IF EXIST %TEST_DIR% (
echo Running tests!
mklink /D %TEMPDIR%\TEST-main ..\%TEST_DIR%
python test_hw.py %TEMPDIR%\main.c
) ELSE (
echo ERROR: No test directory available.
)

cd %TEMPDIR%
rem echo Starting 7zip with arguments: %ProgramFiles%\7-Zip\7z a -y -tzip main.zip main.c
"%ProgramFiles%\7-Zip\7z" a -y -tzip main.zip main.c

cd ..
xcopy %TEMPDIR%\main.zip . /Y /I
pause
echo Removing the temporary directory
rmdir /S /Q %TEMPDIR%
IF %2=="" (
  pause
)