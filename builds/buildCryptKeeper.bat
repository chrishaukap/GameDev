@echo off
echo ------------------------------------
echo USAGE - buildCryptKeeper.bat CONFIGURATION
echo ------------------------------------

set SCRIPT_NAME=buildCryptKeeper.bat
set USAGE_STRING= Usage: %SCRIPT_NAME% configuration
set CONFIGURATION_NAME=%1
echo %CONFIGURATION_NAME%

REM - checkout and build lib_utils
set LIB_UTILS=lib_utils
rem svn co file:///svnsrc/%LIB_UTILS%
cd %LIB_UTILS%/build/vc8
msbuild lib_utils.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..\..

REM - checkout and build lib_ogg
set LIB_OGG=lib_ogg
rem svn co file:///svnsrc/%LIB_OGG%
cd %LIB_OGG%/build
call buildOgg.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout and build lib_vorbis
set LIB_VORBIS=lib_vorbis
rem svn co file:///svnsrc/%LIB_VORBIS%
cd %LIB_VORBIS%/build
call buildVorbis.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout lib_openAL
rem svn co file:///svnsrc/lib_openAL

REM - checkout and build lib_alut
set LIB_ALUT=lib_alut
rem svn co file:///svnsrc/%LIB_ALUT%
cd %LIB_ALUT%/build
call buildAlut.bat %CONFIGURATION_NAME%
cd ..\..

REM - checkout and build lib_oggController
set LIB_OGGCONTROLLER=lib_oggController
rem svn co file:///svnsrc/%LIB_OGGCONTROLLER%
cd %LIB_OGGCONTROLLER%/build/vc8
msbuild lib_oggController.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..\..

REM - checkout and build graveyardDemo
echo ------------------------------------
echo building graveyardDemo in %CONFIGURATION_NAME% 
echo ------------------------------------
set APP_GRAVEYARD=app_graveyardDemo
rem svn co file:///svnsrc/graveyardDemo

cd %APP_GRAVEYARD%/build
cd
dir
msbuild graveyardDemo.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d
cd ..\..

rem echo ------------------------------------
rem echo installer for graveyardDemo
rem echo ------------------------------------

REM - define a script variable to execute against
rem set PROJ_DIR=%cd%\graveyardDemo
rem mkdir graveyardDemo\trunk\dist\vc8\%CONFIGURATION_NAME%
rem makensis /DCONFIGURATION=%CONFIGURATION_NAME% /DTARGET=vc8 "graveyardDemo/trunk/inst/CryptKeeper.nsi

:FAILED_USAGE
ECHO %USAGE_STRING%
:FAILED