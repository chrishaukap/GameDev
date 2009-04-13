@echo off
echo ------------------------------------
echo USAGE - buildAlut.bat CONFIGURATION
echo ------------------------------------

set PROJECT=lib_alut
set CONFIGURATION_NAME=%1

echo ------------------------------------
echo building %PROJECT% in %CONFIGURATION_NAME% 
echo ------------------------------------
msbuild ..\freealut-1.1.0-src\admin\vc8\alut.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d

echo ------------------------------------
echo creating dist for %PROJECT% 
echo ------------------------------------
MKDIR ..\lib\%CONFIGURATION_NAME%
COPY ..\freealut-1.1.0-src\admin\vc8\alut\%CONFIGURATION_NAME%\alut.lib ..\lib\%CONFIGURATION_NAME%\alut.lib


