#pragma once

class b2Shape;
struct Vector2;

class CollisionListener
{
public:
	virtual void OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation)=0;
};
