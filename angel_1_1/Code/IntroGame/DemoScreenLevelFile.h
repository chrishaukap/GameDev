#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"

class DemoScreenLevelFile : public DemoScreen
{
public:
	DemoScreenLevelFile();

	virtual void Start();

private:
	TextActor *t;
	TextActor *t2;
	TextActor *t3;
};