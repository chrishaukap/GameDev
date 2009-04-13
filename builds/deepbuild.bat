@echo off
echo ------------------------------------
echo USAGE - deepbuild.bat PROJECT TARGET CONFIGURATION
echo ------------------------------------

set PROJ_NAME=%1
set TARGET_NAME=%2
set CONFIGURATION_NAME=%3

REM - need a way of defining dependencies
echo ------------------------------------
echo lib_utils
echo ------------------------------------
svn co file:///e:/svnsrc/lib_utils
devenv.com lib_utils\build\%TARGET_NAME%\lib_utils.sln /build %CONFIGURATION_NAME% /project lib_utils
devenv.com lib_utils\build\%TARGET_NAME%\lib_utils.sln /build %CONFIGURATION_NAME% /project validation

echo ------------------------------------
echo lib_dxGameLib
echo ------------------------------------
svn co file:///e:/svnsrc/lib_dxGameLib
devenv.com lib_dxGameLib\build\%TARGET_NAME%\lib_dxGameLib.sln /build %CONFIGURATION_NAME% /project lib_dxGameLib
devenv.com lib_dxGameLib\build\%TARGET_NAME%\lib_dxGameLib.sln /build %CONFIGURATION_NAME% /project adjustsound
devenv.com lib_dxGameLib\build\%TARGET_NAME%\lib_dxGameLib.sln /build %CONFIGURATION_NAME% /project playsound



echo ------------------------------------
echo %PROJ_NAME%
echo ------------------------------------
svn co file:///e:/svnsrc/%PROJ_NAME%
devenv.com %PROJ_NAME%\build\%TARGET_NAME%\%PROJ_NAME%.sln /build %CONFIGURATION_NAME% /project %PROJ_NAME%

echo ------------------------------------
echo inst_tetris
echo ------------------------------------

REM - define a script variable to execute against
set PROJ_DIR=%cd%\%PROJ_NAME%
mkdir %PROJ_DIR%\dist\%TARGET_NAME%\%CONFIGURATION_NAME%
makensis /DCONFIGURATION=%CONFIGURATION_NAME% /DTARGET=%TARGET_NAME% "%PROJ_NAME%/inst/tetrix.nsi