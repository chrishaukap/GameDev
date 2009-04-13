@echo off
echo ------------------------------------
echo USAGE - buildCryptKeeper.bat CONFIGURATION
echo ------------------------------------

set SCRIPT_NAME=buildCryptKeeper.bat
set USAGE_STRING= Usage: %SCRIPT_NAME% configuration
set CONFIGURATION_NAME=%1
if (%CONFIGURATION_NAME%) == () goto FAILED_USAGE 
goto FAILED

REM - checkout and build lib_utils
set LIB_UTILS=lib_utils
svn co file:///svnsrc/%LIB_UTILS%
cd %LIB_UTILS%/build/vc8
msbuild lib_utils.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..\..

REM - checkout and build lib_ogg
set LIB_OGG=lib_ogg
svn co file:///svnsrc/%LIB_OGG%
cd %LIB_OGG%/build
call buildOgg.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout and build lib_vorbis
set LIB_VORBIS=lib_vorbis
svn co file:///svnsrc/%LIB_VORBIS%
cd %LIB_VORBIS%/build
call buildVorbis.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout lib_openAL
svn co file:///svnsrc/lib_openAL

REM - checkout and build lib_alut
set LIB_ALUT=lib_alut
svn co file:///svnsrc/%LIB_ALUT%
cd %LIB_ALUT%/build
call buildAlut.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout and build lib_oggController
set LIB_OGGCONTROLLER=lib_oggController
svn co file:///svnsrc/%LIB_OGGCONTROLLER%
cd %LIB_OGGCONTROLLER%/build
msbuild lib_oggController.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..

REM - checkout and build graveyardDemo
echo ------------------------------------
echo building graveyardDemo in %CONFIGURATION_NAME% 
echo ------------------------------------
set APP_GRAVEYARD=graveyardDemo
svn co file:///svnsrc/graveyardDemo
cd %APP_GRAVEYARD%/trunk/build
msbuild OpenGLApplication.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..\..

echo ------------------------------------
echo installer for graveyardDemo
echo ------------------------------------

REM - define a script variable to execute against
set PROJ_DIR=%cd%\graveyardDemo
mkdir graveyardDemo\trunk\dist\vc8\%CONFIGURATION_NAME%
makensis /DCONFIGURATION=%CONFIGURATION_NAME% /DTARGET=vc8 "graveyardDemo/trunk/inst/CryptKeeper.nsi

:FAILED_USAGE
ECHO %USAGE_STRING%
:FAILED