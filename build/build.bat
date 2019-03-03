@echo off

echo;
echo =========================================
echo Build YaizuDbTool
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
if exist deployment rmdir /S /Q deployment


rem ########## Building ##########
echo;
echo Building stkdatagui.sln...
%MSBUILD% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /t:clean;build /p:Configuration=Release
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED
echo Building sample.sln...
%MSBUILD% "..\src\restapi\sample.sln" /t:clean;build /p:Configuration=Release
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED
echo Building stkwebappcmd.sln...
%MSBUILD% "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.sln" /t:clean;build /p:Configuration=Release
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED


rem ########## Checking file existence ##########
echo;
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" goto ERRORRAISED
echo Checking "nginx-1.12.2.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" goto ERRORRAISED
echo Checking "sample.exe" existence...
if not exist "..\src\restapi\Release\sample.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...
mkdir webapp
mkdir webapp\img
mkdir webapp\manual
mkdir webapp\manual\eng
mkdir webapp\manual\jpn
copy "..\src\resource\*.*" webapp
copy "..\src\resource\img\*.*" webapp\img
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" webapp
copy "..\doc\man\eng\*.*" webapp\manual\eng
copy "..\doc\man\jpn\*.*" webapp\manual\jpn
xcopy /y /q /s "..\src\etc\*.*" webapp
copy "..\LICENSE" webapp\LICENSE.cmdfreak
copy "..\src\restapi\Release\sample.exe" webapp
copy "..\src\restapi\stkwebapp.conf" webapp
copy "..\src\restapi\sample.dat" webapp
copy "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" webapp

mkdir webapp\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" webapp\nginx
pushd webapp\nginx
%SEVENZIP% x "nginx-1.12.2.zip"
popd
xcopy /y /q /i /s /e "webapp\nginx\nginx-1.12.2" webapp
if exist webapp\nginx rmdir /S /Q webapp\nginx


rem ########## Making installer ##########
echo;
echo Making installer...
%DEVENV% "setup\cmdfreak.sln" /rebuild Release
mkdir deployment
copy ..\doc\readme\ReadmeJPN.txt deployment
copy ..\doc\readme\ReadmeENG.txt deployment
copy setup\Release\cmdfreak.msi deployment


rem ########## ZIP packing ##########
echo;
echo ZIP packing stage...
cd deployment
%SEVENZIP% a cfk120.zip cmdfreak.msi
%SEVENZIP% a cfk120.zip ReadmeENG.txt
%SEVENZIP% a cfk120.zip ReadmeJPN.txt
del ReadmeENG.txt
del ReadmeJPN.txt
del cmdfreak.msi
cd..


rem ########## build complete ##########
if not defined APPVEYOR (
  echo;
  %LCOUNTER% ..\src /subdir
  %LCOUNTER% ..\src\resource\index.html /subdir
)
echo;
echo All building processes of CmdFreak have been successfully finished.
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

