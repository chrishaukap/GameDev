#pragma once

#include "TheEndGameManager.h"


class TextActor;
class CutSceneScreen : public GameScreen
{
public:
	CutSceneScreen();

	virtual void Start();
	virtual void Update(float dt);

private:
	TextActor *t;
};