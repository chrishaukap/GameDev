
#include "Tim.h"
#include "MathUtil.h"
#include "TimHead.h"
#include "TimTail.h"
#include "World.h"

#define TIM_SPEED 10.0f

using namespace CDH;
using namespace Edge;

Tim::Tim() : 
   m_head(), 
   m_tail(),
   m_speed(TIM_SPEED)
{}
Tim::~Tim()
{
   Clear();
}

const Vector2 
Tim::GetPosition()
{
	return m_head->GetPosition();
}
const Vector2 
Tim::GetSize()
{
	return m_head->GetSize();
}
void Tim::Init()
{
   m_head = new TimHead();
	m_head->SetSize(1.0f);
	m_head->SetPosition(-10,-10);
	m_head->SetColor(1,0,0);
	theWorld.Add(m_head);

   m_tail = new TimTail(*m_head);
}
void Tim::Clear()
{
   theWorld.Remove(m_head);
   delete m_head; m_head = NULL;
   delete m_tail; m_tail = NULL;
}

void
Tim::GrowTail()
{
   m_tail->addTailNode();
}

static bool g_mouseOn = false;
static Vec2i g_screenCoords;
void Tim::Update(float dt)
{
	if(g_mouseOn)
	{
		const Vector2 screenPos = MathUtil::ScreenToWorld(g_screenCoords);
      Vector2 newPoint = 
			MathUtil::MoveP1TowardP2( m_head->GetPosition(), 
											  screenPos, 
											  m_speed * dt);
		m_head->SetPosition(newPoint);
      m_tail->moveTo(newPoint, m_speed * dt);
	}
}

void Tim::MouseMotionEvent(Vec2i screenCoords)
{
	if(g_mouseOn)
		g_screenCoords = screenCoords;
}

void Tim::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
	g_screenCoords = screenCoords;
	g_mouseOn = true;
}

void Tim::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
	g_mouseOn = false;
}