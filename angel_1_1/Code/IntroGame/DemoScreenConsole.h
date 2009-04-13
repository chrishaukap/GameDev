#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"
#include "Console.h"

class DemoScreenConsole : public DemoScreen, public CCmdMemberFunctorOwner
{
public:
	DemoScreenConsole();

	virtual void Start();
	void ChangeSize(const String& input);

private:
	TextActor *t;
	TextActor *t2;
	TextActor *t3;
	Actor *a;
};