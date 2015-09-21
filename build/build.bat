@echo off

echo;
echo =========================================
echo Build YaizuDbTool
echo =========================================

set CURRENTPATH=%cd%
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe"
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"

echo;
echo This batch file requires softwares shown below.
echo (1) Microsoft Visual Studio 2008 Professional Edition
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
echo Building bbb.sln...
%DEVENV% "..\src\bbb\bbb.sln" /rebuild Release 
echo Building cmdfrksrv.sln...
%DEVENV% "..\src\cmdfrksrv\cmdfrksrv.sln" /rebuild Release 
echo Building prog_add.sln...
%DEVENV% "..\src\prog_add\prog_add.sln" /rebuild Release
echo Building prog_del.sln...
%DEVENV% "..\src\prog_del\prog_del.sln" /rebuild Release
echo Building stkdatagui.sln...
%DEVENV% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /rebuild Release
echo Building srvcmd.sln...
%DEVENV% "..\src\srvcmd\srvcmd.sln" /rebuild Release


rem ########## Checking file existence ##########
echo;
echo Checking "bbb.exe" existence...
if not exist "..\src\bbb\Release\bbb.exe" goto FILENOTEXIST
echo Checking "cmdfrksrv.exe" existence...
if not exist "..\src\cmdfrksrv\Release\cmdfrksrv.exe" goto FILENOTEXIST
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" goto FILENOTEXIST
echo Checking "srvcmd.exe" existence...
if not exist "..\src\srvcmd\Release\srvcmd.exe" goto FILENOTEXIST
echo Checking "prog_add.exe" existence...
if not exist "..\src\prog_add\Release\prog_add.exe" goto FILENOTEXIST
echo Checking "prog_del.exe" existence...
if not exist "..\src\prog_del\Release\prog_del.exe" goto FILENOTEXIST


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
copy "..\src\bbb\Release\bbb.exe" webapp
copy "..\src\cmdfrksrv\Release\cmdfrksrv.exe" webapp
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" webapp
copy "..\src\srvcmd\Release\srvcmd.exe" webapp
copy "..\src\prog_add\Release\prog_add.exe" setup
copy "..\src\prog_del\Release\prog_del.exe" setup
copy "..\doc\man\eng\*.*" webapp\manual\eng
copy "..\doc\man\jpn\*.*" webapp\manual\jpn
xcopy /y /q /s "..\src\etc\*.*" webapp


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
%SEVENZIP% a cfk100.zip cmdfreak.msi
%SEVENZIP% a cfk100.zip ReadmeENG.txt
%SEVENZIP% a cfk100.zip ReadmeJPN.txt
del ReadmeENG.txt
del ReadmeJPN.txt
del cmdfreak.msi
cd..


rem ########## build complete ##########
echo;
%LCOUNTER% ..\src /subdir
echo;
echo All building processes of CmdFreak have been successfully finished.
exit /B


rem ########## Error ##########
:FILENOTEXIST
echo;
echo Build error occurred because some build target files do not exist.
exit /B

