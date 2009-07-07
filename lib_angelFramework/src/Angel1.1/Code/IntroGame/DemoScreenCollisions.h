#pragma once

#include "DemoGameManager.h"

#include "TextActor.h"
#include "PhysicsActor.h"
#include "PhysicsUtil.h"
#include "Sound.h"

class DemoScreenCollisions : public DemoScreen, public CollisionListener
{
public:
	DemoScreenCollisions();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float dt);

	virtual void OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation);

private:
	TextActor* t;
	PhysicsActor *p1;
	PhysicsActor *p2;

	Sound *sound;
};