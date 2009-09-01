
#include "Jake.h"
#include "MathUtil.h"
#include "World.h"
#include "JakeGameManager.h"

using namespace CDH;
using namespace Chubdiso;
#define JAKE_SPEED 10

Jake::Jake() :
	m_speed(JAKE_SPEED)
{}
Jake::~Jake()
{
   Clear();
}

void Jake::Init()
{
   SetSize(2);

   LoadSpriteFrames("Resources/Images/Angel_01.png");
   PlaySpriteAnimation(1.0f, SAT_Loop, 0, 1);   
	SetPosition(0.0f, 0.0f);
	theWorld.Add(this,1);
}
void Jake::Clear()
{
   theWorld.Remove(this);
}


static bool g_mouseOn = false;
static Vec2i g_screenCoords;
void Jake::Update(float dt)
{
   UpdateSpriteAnimation(dt);
	if(g_mouseOn)
	{
		SetPosition(
			MathUtil::MoveP1TowardP2( GetPosition(), 
											  MathUtil::ScreenToWorld(g_screenCoords), 
											  m_speed * dt));
	}
}

void Jake::MouseMotionEvent(Vec2i screenCoords)
{
	if(g_mouseOn)
		g_screenCoords = screenCoords;
}

void Jake::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
	g_screenCoords = screenCoords;
	g_mouseOn = true;
}

void Jake::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
	g_mouseOn = false;
}