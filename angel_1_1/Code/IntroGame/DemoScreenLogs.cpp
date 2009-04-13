#include "stdafx.h"
#include "DemoScreenLogs.h"

#include "World.h"
#include "TextActor.h"
#include "Log.h"

DemoScreenLogs::DemoScreenLogs()
{
	//You can make as many log files as you want to store off different
	//  information. 
	_pFileLog = new FileLog( FileLog::MakeLogFileName( "LogFile" ) );
}

DemoScreenLogs::~DemoScreenLogs()
{
	delete _pFileLog;
}

void DemoScreenLogs::Start()
{
	//Declaring our console command (see DemoScreenConsole.cpp for more 
	//  information on such things.)
	CONSOLE_DECLARECMDMETHOD( PrintToFileLog, this, DemoScreenLogs, PrintToFileLog );


	//Just some TextActors explaining what to do. 
	TextActor *t = new TextActor("Console", "The console also supports a log.  Type \"ECHO <your message>\"");
	t->SetPosition(0, 3.5);
	t->SetAlignment(TXT_Center);
	TextActor *t2 = new TextActor("Console", "Type \"PRINTTOFILELOG <your message>\" to print to Logs\\Logfile.log.");
	t2->SetPosition(0, 2);
	t2->SetAlignment(TXT_Center);
	TextActor *t3 = new TextActor("Console", "(This is particularly useful when trying to debug your config files.)");
	t3->SetPosition(0, -1);
	t3->SetAlignment(TXT_Center);

	theWorld.Add(t);
	theWorld.Add(t2);
	theWorld.Add(t3);





	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenLogs.cpp, LogFile.log");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(t3);
	#pragma endregion
}

void DemoScreenLogs::PrintToFileLog( const String& input )
{
	//Just pass any string to the log function and it gets written into the file.
	//  Simple, but now you don't have to do the file BS yourself. :-)
	_pFileLog->Log( input );
}
