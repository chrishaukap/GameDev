#include "stdafx.h"
#include "CVarChangedAIEvent.h"
#include "Brain.h"
#include "Console.h"


OnConsoleVariableChangedAIEvent* OnConsoleVariableChangedAIEvent::Initialize( const String& cVarId )
{
	_variable = theConsole.GetConsoleItemManager().GetCVar( cVarId );
	if( _variable )
		_lastValue = _variable->GetStringVal();

	return this;
}

void OnConsoleVariableChangedAIEvent::Update(float dt)
{
	if( _variable == NULL )
		return;

	if( _variable->GetStringVal() != _lastValue )
	{
		IssueCallback();
	}
}
