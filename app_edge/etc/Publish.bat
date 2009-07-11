
..\..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe . ..\..\Published /IF FreeImage.dll /NJH /NJS /NP /NS /NC /NDL
rem ..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe . ..\Published /IF GameInfo.txt /NJH /NJS /NP /NS /NC /NDL
..\..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe ..\..\res ..\..\Published\Resources /MIR /XD CVS /NJH /NJS /NP /NS /NC /NDL
..\..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe ..\..\Config ..\..\Published\Config /MIR /XD CVS /NJH /NJS /NP /NS /NC /NDL
rem ..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe Logs Published\Logs /MIR /XD .svn /NJH /NJS /NP /NS /NC /NDL
..\..\..\lib_angelFramework\src\Angel1.1\Code\Tools\Robocopy\robocopy.exe ..\..\bin\win32\Release ..\..\Published /IF *.exe /NJH /NJS /NP /NS /NC /NDL


rem echo Errorlevel = %ERRORLEVEL%

rem Robocopy returns errorlevel as bitmask
rem 16	FATAL ERROR
rem  8	FAILED COPIES
rem  4	MISMATCHES
rem  2	EXTRA FILES
rem  1	Copy successful
rem	 0  No Change

rem We'll assume that below 8 is ok.
if %ERRORLEVEL% LEQ 7 EXIT /B 0

EXIT /B %ERRORLEVEL%
