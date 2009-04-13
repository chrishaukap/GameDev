ECHO copying image and sources to debug folder
xcopy /I/Y/S/E ..\..\src\img ..\..\bin\vc7\%1\img
xcopy /I/Y/S/E ..\..\src\sounds ..\..\bin\vc7\%1\sounds
xcopy /I/Y/S/E ..\..\src\testing\run.bat ..\..\bin\vc7\%1
xcopy /I/Y/S/E ..\..\src\vc7\cfg.txt ..\..\bin\vc7\%1