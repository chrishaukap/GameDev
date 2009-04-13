#pragma once

#include "DemoGameManager.h"
#include "Console.h"

class FileLog;

class DemoScreenLogs : public DemoScreen, public CCmdMemberFunctorOwner
{
public:
	DemoScreenLogs();
	~DemoScreenLogs();

	virtual void Start();
	void PrintToFileLog( const String& input );

private:
	FileLog*	_pFileLog;
};