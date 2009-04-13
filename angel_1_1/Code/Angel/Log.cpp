#include "stdafx.h"
#include "Log.h"
#include "MathUtil.h"
#include "FileUtil.h"
#include "Console.h"

#include <stdio.h>
#include <time.h>

void CompoundLog::AddLog( DeveloperLog* addLog )
{
	//Add unique
	for( int i = 0; i < _logs.size(); i++ )
	{
		if( addLog == _logs[i] )
			return;
	}

	_logs.push_back( addLog );
}

void CompoundLog::Log( const String& val )
{
	for( int i = 0; i < _logs.size(); i++ )
	{
		_logs[i]->Log( val );
	}
}


void ConsoleLog::Log( const String& val)
{
	//Split by lines
	StringList splitByLines = SplitString(val, "\n" );
	for( int i = 0; i < splitByLines.size(); i++ )
	{
		_logLines.push_back( splitByLines[i] );
	}
}

const String& ConsoleLog::GetLogLine( int lineIndex )
{
	static String emptyLine;
	if( _logLines.size() == 0 )
		return emptyLine;

	lineIndex = MathUtil::Clamp( lineIndex, 0, (int)(_logLines.size() - 1) );
	return _logLines[lineIndex];
}

int ConsoleLog::NumLogLines()
{
	return _logLines.size();
}

String FileLog::MakeLogFileName( const String& fileName )
{
	CONSOLE_DECLAREVAR( g_logDirectory );

	String logDir = "Logs//";
	if( g_logDirectory->HasVal() )
	{
		//overwrite log dir
		logDir = g_logDirectory->GetStringVal();
	}

	return logDir + fileName + ".log";
}


FileLog::FileLog( const String& fileName )
: _fileName(fileName)
{
	time_t rawtime;
	time ( &rawtime );

	//TODO: may want to backup the old log?
	//Clear the current log
	StringList logHeader;
	logHeader.push_back( String("Opened Log: ") + _fileName );
	logHeader.push_back( String("On: ") + ctime(&rawtime) );
	WriteLinesToFile( _fileName, logHeader );

	
}

void FileLog::Log( const String& val)
{
	AppendLineToFile( _fileName, val );
}


