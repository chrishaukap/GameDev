#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"

class DemoScreenSimpleActor : public DemoScreen
{
public:
	DemoScreenSimpleActor();

	virtual void Start();
	virtual void Update(float dt);

private:
	TextActor *t;
	Actor *a;
};