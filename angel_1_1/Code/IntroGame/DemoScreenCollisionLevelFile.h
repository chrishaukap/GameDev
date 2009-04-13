#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"

class DemoScreenCollisionLevelFile : public DemoScreen
{
public:
	DemoScreenCollisionLevelFile();

	virtual void Start();

private:
	TextActor *t;
	TextActor *t2;
	TextActor *t3;
	TextActor *t4;
};