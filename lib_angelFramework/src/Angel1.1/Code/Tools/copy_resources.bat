..\Tools\Robocopy\robocopy.exe ..\Angel\Libraries\FreeImage\Dist\ ..\%1 /IF FreeImage.dll /NJH /NJS /NP /NS /NC /NDL
..\Tools\Robocopy\robocopy.exe Resources ..\%1\Resources /MIR /XD .svn /NJH /NJS /NP /NS /NC /NDL
..\Tools\Robocopy\robocopy.exe Config ..\%1\Config /MIR /XD .svn /NJH /NJS /NP /NS /NC /NDL

rem echo Errorlevel = %ERRORLEVEL%

rem Robocopy returns errorlevel as bitmask
rem 16	FATAL ERROR
rem  8	FAILED COPIES
rem  4	MISMATCHES
rem  2	EXTRA FILES
rem  1	Copy successful
rem	 0  No Change

echo --------------------------------------------------------------

rem We'll assume that below 8 is ok.
if %ERRORLEVEL% LEQ 7 EXIT /B 0

EXIT /B %ERRORLEVEL%
