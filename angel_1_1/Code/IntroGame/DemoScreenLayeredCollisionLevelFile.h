#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"

class DemoScreenLayeredCollisionLevelFile : public DemoScreen
{
public:
	DemoScreenLayeredCollisionLevelFile();

	virtual void Start();

private:
	TextActor *t;
	TextActor *t2;
	TextActor *t3;
	TextActor *t4;
};