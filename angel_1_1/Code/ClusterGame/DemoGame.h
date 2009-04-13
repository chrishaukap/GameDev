#pragma once
#include "Actor.h"
#include "FullScreenActor.h"
#include "PhysicsUtil.h"
#include "GameManager.h"
#include "MouseInput.h"

// forward declarations
class PhysicsActor; 
class PhysicsEventActor;
struct Sound;


enum renderLayer
{
	RL_Background,
	RL_World,
	RL_Foreground,
	RL_UI
};

class DemoGame : public GameManager, public MouseListener
{
public:
	DemoGame();
	~DemoGame();

	virtual void Render();
	virtual void Update(float dt);
	virtual void OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation);

	virtual void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
	virtual bool IsProtectedFromUnloadAll( Renderable* pRenderable );

private:
	Actor *_demo1;
	Actor *_demo2;
	Actor *_demo3;
	Actor *_demo4;
	Actor *_demo5;
	Actor *_demo6;
	PhysicsEventActor *_physGround;
	PhysicsEventActor *_physDemo1;
	PhysicsActor *_physDemo2;

	FullScreenActor *_fullScreen;
	Sound*	_firstSound;

	double _simTime;
};
