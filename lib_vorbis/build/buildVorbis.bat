@echo off
echo ------------------------------------
echo USAGE - buildVorbis.bat CONFIGURATION
echo ------------------------------------

set PROJECT=lib_vorbis
set CONFIGURATION_NAME=%1

echo ------------------------------------
echo building %PROJECT% in %CONFIGURATION_NAME% 
echo ------------------------------------
msbuild ..\libvorbis-1.2.0\win32\vorbis.sln /t:Rebuild /p:Configuration=%CONFIGURATION_NAME% /v:d

echo ------------------------------------
echo creating dist for %PROJECT% 
echo ------------------------------------
IF %CONFIGURATION_NAME%==Debug (
	MKDIR ..\lib\Debug
	COPY ..\libvorbis-1.2.0\win32\Vorbis_Static_Debug\vorbis.lib ..\lib\Debug\vorbis.lib
	COPY ..\libvorbis-1.2.0\win32\Vorbis_Dynamic_Debug\vorbis.dll ..\lib\Debug\vorbis.dll
	COPY ..\libvorbis-1.2.0\win32\VorbisFile_Static_Debug\vorbisfile.lib ..\lib\Debug\vorbisfile.lib
	COPY ..\libvorbis-1.2.0\win32\VorbisFile_Dynamic_Debug\vorbisfile.dll ..\lib\Debug\vorbisfile.dll	
)
IF %CONFIGURATION_NAME%==Release (
	MKDIR ..\lib\Release
	COPY ..\libvorbis-1.2.0\win32\Vorbis_Static_Release\vorbis.lib ..\lib\Release\vorbis.lib
	COPY ..\libvorbis-1.2.0\win32\Vorbis_Dynamic_Release\vorbis.dll ..\lib\Release\vorbis.dll	
	COPY ..\libvorbis-1.2.0\win32\VorbisFile_Static_Release\vorbisfile.lib ..\lib\Release\vorbisfile.lib
	COPY ..\libvorbis-1.2.0\win32\VorbisFile_Dynamic_Release\vorbisfile.dll ..\lib\Release\vorbisfile.dll	
)


