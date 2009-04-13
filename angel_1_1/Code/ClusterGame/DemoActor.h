#pragma once

#include "Actor.h"

class DemoActor : public Actor
{
public:
	virtual void Update(float dt);
	virtual void Render();
	virtual void AnimCallback(String animName);
};
