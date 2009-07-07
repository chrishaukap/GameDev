#pragma once

#include "Actor.h"
#include "PhysicsUtil.h"

// forward declarations
class b2Shape;

class GameManager : public Renderable, public CollisionListener
{
public:
	GameManager(){}
	virtual ~GameManager(){}

	virtual void Render();
	virtual void Update(float dt);
   virtual void CleanUp(){}
	virtual void OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation);
	virtual bool IsProtectedFromUnloadAll( Renderable* renderable ) {return false;}
};
