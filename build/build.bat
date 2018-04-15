@echo off

echo;
echo =========================================
echo Build YaizuDbTool
echo =========================================

set CURRENTPATH=%cd%
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"

echo;
echo This batch file requires softwares shown below.
echo (1) Microsoft Visual Studio 2017
echo (2) 7-Zip 9.20
echo (3) Line Counter

if not exist %DEVENV% exit
if not exist %SEVENZIP% exit
if not exist %LCOUNTER% exit


rem ########## Initializing ##########
echo;
echo Initializing...
if exist webapp rmdir /S /Q webapp
if exist deployment rmdir /S /Q deployment

rem ########## Building ##########
echo;
echo Building stkdatagui.sln...
%DEVENV% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /rebuild Release
echo Building sample.sln...
%DEVENV% "..\src\restapi\sample.sln" /rebuild Release
echo Building stkwebappcmd.sln...
%DEVENV% "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.sln" /rebuild Release


rem ########## Checking file existence ##########
echo;
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" goto FILENOTEXIST
echo Checking "nginx-1.12.2.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" goto FILENOTEXIST
echo Checking "stkwebapp.exe" existence...
if not exist "..\src\restapi\Release\stkwebapp.exe" goto FILENOTEXIST
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" goto FILENOTEXIST


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
copy "..\src\restapi\Release\stkwebapp.exe" webapp
copy "..\src\restapi\stkwebapp.conf" webapp
copy "..\src\restapi\stkwebapp.dat" webapp
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
copy setup\Release\cmdfreak.msi deployment


rem ########## ZIP packing ##########
echo;
echo ZIP packing stage...
copy ..\doc\readme\ReadmeJPN.txt deployment
copy ..\doc\readme\ReadmeENG.txt deployment
cd deployment
%SEVENZIP% a cfk120.zip cmdfreak.msi
%SEVENZIP% a cfk120.zip ReadmeENG.txt
%SEVENZIP% a cfk120.zip ReadmeJPN.txt
del ReadmeENG.txt
del ReadmeJPN.txt
del cmdfreak.msi
cd..


rem ########## build complete ##########
echo;
%LCOUNTER% ..\src /subdir
%LCOUNTER% ..\src\resource\index.html /subdir
echo;
echo All building processes of CmdFreak have been successfully finished.
pause
exit /B


rem ########## Error ##########
:FILENOTEXIST
echo;
echo Build error occurred because some build target files do not exist.
pause
exit /B

