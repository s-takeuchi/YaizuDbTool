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
if exist deployment\cfk120.zip rmdir /S /Q deployment\cfk120.zip


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
echo Checking "jquery-3.2.0.min.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" goto ERRORRAISED
echo Checking "IcoMoon-Free.ttf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" goto ERRORRAISED
echo Checking "bootstrap-4.4.1-dist.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" goto ERRORRAISED
echo Checking "stkcommon.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" goto ERRORRAISED
echo Checking "sample.exe" existence...
if not exist "..\src\restapi\Release\sample.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.conf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...
mkdir webapp
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" webapp
copy "..\LICENSE" webapp\LICENSE.cmdfreak
copy "..\src\restapi\Release\sample.exe" webapp
copy "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" webapp
copy "..\src\restapi\sample.dat" webapp
copy "..\src\restapi\sample.conf" webapp
copy "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" webapp

mkdir webapp\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" webapp\nginx
pushd webapp\nginx
%SEVENZIP% x "nginx-1.12.2.zip"
popd
xcopy /y /q /i /s /e "webapp\nginx\nginx-1.12.2" webapp
if exist webapp\nginx rmdir /S /Q webapp\nginx

mkdir webapp\bootstrap
copy "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" webapp\bootstrap
pushd webapp\bootstrap
%SEVENZIP% x "bootstrap-4.4.1-dist.zip"
popd
xcopy /y /q /i /s /e "webapp\bootstrap\bootstrap-4.4.1-dist" webapp\html\bootstrap-4.4.1-dist
if exist webapp\bootstrap rmdir /S /Q webapp\bootstrap

mkdir webapp\license
mkdir webapp\html\lib
mkdir webapp\html\img
mkdir webapp\html\manual
mkdir webapp\html\manual\eng
mkdir webapp\html\manual\jpn
copy "..\src\resource\*.*" webapp\html
copy "..\src\resource\img\*.*" webapp\html\img
copy "..\doc\man\eng\*.*" webapp\html\manual\eng
copy "..\doc\man\jpn\*.*" webapp\html\manual\jpn
xcopy /y /q /s "..\src\etc\lib" webapp\html\lib
xcopy /y /q /s "..\src\etc\license" webapp\license
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.css" webapp\html


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
  %LCOUNTER% ..\src\resource\cmdfreak.js
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

