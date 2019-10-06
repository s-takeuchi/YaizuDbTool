@echo off

echo;
echo =========================================
echo Test YaizuDbTool
echo =========================================

if defined APPVEYOR (
  set MSBUILD="msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
)

if not defined APPVEYOR (
  set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
  set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
  set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"
)

if not defined APPVEYOR (
  echo;
  echo This batch file requires softwares shown below.
  echo 1. Microsoft Visual Studio 2017
  echo 2. 7-Zip 9.20
  echo 3. Line Counter

  if not exist %MSBUILD% (
    exit
  ) else if not exist %DEVENV% (
    exit
  ) else if not exist %SEVENZIP% (
    exit
  ) else if not exist %LCOUNTER% (
    exit
  )
)


rem ########## Initializing ##########
echo;
echo Initializing...
if exist webapp rmdir /S /Q webapp


rem ########## Building ##########
echo;
echo Building sample.sln...
rem Considering execution privilege, .exe is built as Debug. (Debug=AsInvoker, Release=AsAdmin)
%MSBUILD% "..\src\restapi\sample.sln" /t:clean;build /p:Configuration=Debug
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED
echo Building restapitest.sln...
%MSBUILD% "..\test\restapitest\restapitest.sln" /t:clean;build /p:Configuration=Release
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED


rem ########## Checking file existence ##########
echo;
echo Checking "sample.exe" existence...
if not exist "..\src\restapi\Debug\sample.exe" goto ERRORRAISED
echo Checking "restapitest.exe" existence...
if not exist "..\test\restapitest\Release\restapitest.exe" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...
mkdir webapp
copy "..\src\restapi\Debug\sample.exe" webapp
copy "..\src\restapi\sample.dat" webapp
copy "..\src\restapi\sample.conf" webapp
echo servicehost=localhost>> webapp\sample.conf
echo serviceport=2060>> webapp\sample.conf
copy "..\test\restapitest\Release\restapitest.exe" webapp


rem ########## Testing ##########
echo;
echo Test starts
start webapp\sample.exe
timeout /t 3
webapp\restapitest.exe
echo Test ends


rem ########## build complete ##########
echo;
echo All testing processes of CmdFreak have been successfully finished.
if not defined APPVEYOR (
  pause
)
exit /B %ERRORLEVEL%


rem ########## Error ##########
:ERRORRAISED
echo;
echo Build error.
if not defined APPVEYOR (
  pause
)
exit /B 1
