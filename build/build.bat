@echo off

echo;
echo =========================================
echo Build CmdFreak
echo =========================================

set CURRENTPATH=%cd%
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe"


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
%DEVENV% "..\src\stkdatagui\stkdatagui.sln" /rebuild Release
echo Building srvcmd.sln...
%DEVENV% "..\src\srvcmd\srvcmd.sln" /rebuild Release


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
rem xcopy /y /q /s "..\..\data\webapp\*.*" webapp

exit /B


rem ########## Making installer ##########
echo;
echo Digital signing (1st)...
if exist ..\stkcodesign.pfx "C:\Program Files\Microsoft Platform SDK\Bin\signtool" sign /f ..\stkcodesign.pfx /a /t "http://timestamp.globalsign.com/scripts/timstamp.dll" /p happ1975 webapp\bbb.exe
if exist ..\stkcodesign.pfx "C:\Program Files\Microsoft Platform SDK\Bin\signtool" sign /f ..\stkcodesign.pfx /a /t "http://timestamp.globalsign.com/scripts/timstamp.dll" /p happ1975 webapp\cmdfrksrv.exe
if exist ..\stkcodesign.pfx "C:\Program Files\Microsoft Platform SDK\Bin\signtool" sign /f ..\stkcodesign.pfx /a /t "http://timestamp.globalsign.com/scripts/timstamp.dll" /p happ1975 webapp\stkdatagui.exe
if exist ..\stkcodesign.pfx "C:\Program Files\Microsoft Platform SDK\Bin\signtool" sign /f ..\stkcodesign.pfx /a /t "http://timestamp.globalsign.com/scripts/timstamp.dll" /p happ1975 webapp\srvcmd.exe

echo;
echo Making installer...
%DEVENV% "setup\cmdfreak.sln" /rebuild Release
mkdir deployment
copy setup\Release\cmdfreak.msi deployment

echo;
echo Digital signing (2nd)...
if exist ..\stkcodesign.pfx "C:\Program Files\Microsoft Platform SDK\Bin\signtool" sign /f ..\stkcodesign.pfx /a /t "http://timestamp.globalsign.com/scripts/timstamp.dll" /p happ1975 deployment\cmdfreak.msi


rem ########## ZIP packing ##########
echo;
echo ZIP packing stage...
copy ReadmeJPN.txt deployment
copy ReadmeENG.txt deployment
cd deployment
..\..\7za.exe a cfk100.zip cmdfreak.msi
..\..\7za.exe a cfk100.zip ReadmeENG.txt
..\..\7za.exe a cfk100.zip ReadmeJPN.txt
del ReadmeENG.txt
del ReadmeJPN.txt
del cmdfreak.msi
cd..


rem ########## build complete ##########
echo;
echo All building processes of CmdFreak have been successfully finished.
exit /B
