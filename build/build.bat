@echo off

echo;
echo =========================================
echo Build YaizuDbTool
echo =========================================

if defined GITHUBACTIONS (
  echo For GitHub Actions
  set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\msbuild.exe"
  set DEVENV="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.com"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
  goto definitionend
)

set LOCALMACHINE="true"

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\msbuild.exe"
set DEVENV="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com"
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"

echo;
echo This batch file requires softwares shown below.
echo 1. Microsoft Visual Studio 2022
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

:definitionend


rem ########## Initializing ##########
echo;
echo Initializing...
if exist webapp rmdir /S /Q webapp
if exist deployment\cfk120.zip del deployment\cfk120.zip


rem ########## Building ##########
echo;
echo Building stkdatagui.sln...
%MSBUILD% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /t:clean;build /p:Configuration=Release /p:platform="x64"
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED
echo Building cmdfreak.sln...
%MSBUILD% "..\src\restapi\cmdfreak.sln" /t:clean;build /p:Configuration=Release /p:platform="x64"
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED
echo Building stkwebappcmd.sln...
%MSBUILD% "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.sln" /t:clean;build /p:Configuration=Release /p:platform="x64"
IF %ERRORLEVEL% NEQ 0 goto ERRORRAISED


rem ########## Checking file existence ##########
echo;
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\x64\Release\stkdatagui.exe" goto ERRORRAISED
echo Checking "nginx-1.28.1.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.28.1.zip" goto ERRORRAISED
echo Checking "jquery-3.2.0.min.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" goto ERRORRAISED
echo Checking "IcoMoon-Free.ttf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" goto ERRORRAISED
echo Checking "bootstrap-4.4.1-dist.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" goto ERRORRAISED
echo Checking "stkcommon.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" goto ERRORRAISED
echo Checking "stkcommon.css" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.css" goto ERRORRAISED
echo Checking "stkcommon_um.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp_um\stkcommon_um.js" goto ERRORRAISED
echo Checking "cmdfreak.exe" existence...
if not exist "..\src\restapi\x64\Release\cmdfreak.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\x64\Release\stkwebappcmd.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.conf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir webapp
copy "..\..\YaizuComLib\src\stkdatagui\x64\Release\stkdatagui.exe" webapp
copy "..\LICENSE" webapp\LICENSE.cmdfreak
copy "..\src\restapi\x64\Release\cmdfreak.exe" webapp
copy "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" webapp
copy "..\src\restapi\cmdfreak.dat" webapp
copy "..\src\restapi\cmdfreak.conf" webapp
copy "..\..\YaizuComLib\src\stkwebapp\x64\Release\stkwebappcmd.exe" webapp

mkdir webapp\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.28.1.zip" webapp\nginx
pushd webapp\nginx
%SEVENZIP% x "nginx-1.28.1.zip"
popd
xcopy /y /q /i /s /e "webapp\nginx\nginx-1.28.1" webapp
if exist webapp\nginx rmdir /S /Q webapp\nginx

mkdir webapp\bootstrap
copy "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" webapp\bootstrap
pushd webapp\bootstrap
%SEVENZIP% x "bootstrap-4.4.1-dist.zip"
popd
xcopy /y /q /i /s /e "webapp\bootstrap\bootstrap-4.4.1-dist" webapp\html\bootstrap-4.4.1-dist
if exist webapp\bootstrap rmdir /S /Q webapp\bootstrap

mkdir webapp\license
mkdir webapp\html\img
copy "..\src\resource\*.*" webapp\html
copy "..\src\resource\img\*.*" webapp\html\img
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.css" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp_um\stkcommon_um.js" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" webapp\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.css" webapp\html


rem ########## Making installer ##########
if defined LOCALMACHINE (
  echo;
  echo Making installer...
  %DEVENV% "setup\cmdfreak.sln" /rebuild Release
  if not exist deployment mkdir deployment
  copy ..\doc\readme\ReadmeJPN.html deployment
  copy ..\doc\readme\ReadmeENG.html deployment
  copy setup\Release\cmdfreak.msi deployment
)


rem ########## ZIP packing ##########
if defined LOCALMACHINE (
  echo;
  echo ZIP packing stage...
  cd deployment
  %SEVENZIP% a cfk120.zip cmdfreak.msi
  %SEVENZIP% a cfk120.zip ReadmeENG.html
  %SEVENZIP% a cfk120.zip ReadmeJPN.html
  del ReadmeENG.html
  del ReadmeJPN.html
  del cmdfreak.msi
  cd..
)


rem ########## build complete ##########
if defined LOCALMACHINE (
  echo;
  %LCOUNTER% ..\src /subdir
)

echo;
echo All building processes of CmdFreak have been successfully finished.
if defined LOCALMACHINE (
  pause
)
exit /B %ERRORLEVEL%


rem ########## Error ##########
:ERRORRAISED
echo;
echo Build error.
if defined LOCALMACHINE (
  pause
)
exit /B 1

