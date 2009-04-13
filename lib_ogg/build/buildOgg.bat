@echo off
echo ------------------------------------
echo USAGE - buildOgg.bat CONFIGURATION
echo ------------------------------------

set PROJECT=lib_ogg
set CONFIGURATION_NAME=%1

echo ------------------------------------
echo building %PROJECT% in %CONFIGURATION_NAME% 
echo ------------------------------------
msbuild ..\libogg-1.1.3\win32\ogg.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d

echo ------------------------------------
echo creating dist for %PROJECT% 
echo ------------------------------------
IF %CONFIGURATION_NAME%==Debug (
	MKDIR ..\lib\Debug
	COPY ..\libogg-1.1.3\win32\Static_Debug\ogg.lib ..\lib\Debug\ogg.lib
	COPY ..\libogg-1.1.3\win32\Dynamic_Debug\ogg.dll ..\lib\Debug\ogg.dll	
)
IF %CONFIGURATION_NAME%==Release (
	MKDIR ..\lib\Release
	COPY ..\libogg-1.1.3\win32\Static_Release\ogg.lib ..\lib\Release\ogg.lib
	COPY ..\libogg-1.1.3\win32\Dynamic_Release\ogg.dll ..\lib\Release\ogg.dll	
)


