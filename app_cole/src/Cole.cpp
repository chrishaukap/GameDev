
#include "Cole.h"
#include "MathUtil.h"
#include "World.h"
#include "ColeGameManager.h"

using namespace CDH;
using namespace Cole;
#define DefaultSpeed 15.0f

ColeAvatar::ColeAvatar() :
	m_speed(DefaultSpeed)
{}
ColeAvatar::~ColeAvatar()
{
   Clear();
}

void ColeAvatar::Init()
{
   SetSize(2);
   //SetSprite("Resources/Images/Angel_01.png", 0);
   //SetSprite("Resources/Images/Angel_02.png", 1);

   LoadSpriteFrames("Resources/Images/Angel_01.png");
   PlaySpriteAnimation(1.0f, SAT_Loop, 0, 1);
   
//	SetColor(1.0f, 0.0f, 0.0f);
	SetPosition(0.0f, 0.0f);

	InitPhysics( 0.8f, //density (0.0f will make it an immovable object)
					 5.0f, //friction
					 0.0f, //restitution (bounciness)
					 PhysicsActor::SHAPETYPE_BOX //we support box and circle types (box is default)
					 );
	theWorld.Add(this);
}
void ColeAvatar::Clear()
{
   theWorld.Remove(this);
}

static const int gJumpTimerCap = 200;
static int gJumpingTimer = gJumpTimerCap;
void ColeAvatar::Update(float dt)
{
   UpdateSpriteAnimation(dt);
	++gJumpingTimer;
	if(theColeGame.isADown())
		moveLeft();
	if(theColeGame.isDDown())
		moveRight();
	if(theColeGame.isWDown())
		jump();
}

static const float gJumpForce = 1000.0f;
void ColeAvatar::jump()
{
	if( gJumpingTimer >= gJumpTimerCap )
	{
		ApplyForce(Vector2(0.0f, gJumpForce), Vector2());
		gJumpingTimer = 0;
	}
}

static const float gMovementForce = 60.0f;
void ColeAvatar::moveLeft()
{
	if(gJumpingTimer >= gJumpTimerCap)
	{
		Vector2 linearVelocity;
		GetLinearVelocity(linearVelocity);
		if(linearVelocity.X >= -m_speed)
			ApplyForce(Vector2(-gMovementForce, 0.0f), Vector2());
	}
}
void ColeAvatar::moveRight()
{
	if(gJumpingTimer >= gJumpTimerCap)
	{
		Vector2 linearVelocity;
		GetLinearVelocity(linearVelocity);
		if(linearVelocity.X <= m_speed)
			ApplyForce(Vector2(gMovementForce, 0.0f), Vector2());	
	}
}

