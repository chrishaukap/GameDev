#include "cdhBase.h"
#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "../Planet.h"
#include "Infrastructure/World.h"

#define Worm_SPEED 10.0f
static const int WORM_MAX_NODES = 15;
static const float ORBIT_DISTANCE = 3;

Worm::Worm() : 
   m_head(), 
   m_tail(),
   m_speed(Worm_SPEED),
   m_planetOrbit(NULL)
{}
Worm::~Worm()
{
   Clear();
}

const Vector2 
Worm::GetPosition()
{
	return m_head->GetPosition();
}
const Vector2 
Worm::GetSize()
{
	return m_head->GetSize();
}
void Worm::Init()
{
   m_head = new WormHead();
	m_head->SetSize(1.0f);
	m_head->SetPosition(-10,-10);
	m_head->SetColor(1,1,1);
   
   m_tail = new WormTail(*m_head, WORM_MAX_NODES);
}
void Worm::Clear()
{
   theWorld.Remove(m_head);
   delete m_head; m_head = NULL;
   delete m_tail; m_tail = NULL;
}

void
Worm::GrowTail()
{
   m_tail->addTailNode();
}
void Worm::AddToWorld()
{
   theWorld.Add(m_head);
   m_tail->AddToWorld();
}
static Vec2i g_screenCoords;
static Vector2 g_orbitPointVec = Vector2(ORBIT_DISTANCE,0);

enum MoveState
{
   NotMoving, Mouse, PlanetClick, PlanetOrbit
};
static MoveState g_moveState = NotMoving;
static Actor *g_a = new Actor;

void Worm::MoveInDirection(Vector2& direction, float distance)
{
   direction.Normalize();
   Vector2 newPoint = Vector2( m_head->GetPosition() + (direction * distance) );	

   m_head->SetPosition(newPoint);
   m_tail->moveTo(newPoint, distance);
}
void Worm::Update(float dt)
{
   g_a->SetColor(0,1,0,1);
   g_a->SetSize(0.125f);
   Vector2 targetPos( 
      m_planetOrbit == NULL ? Vector2(0,0) : 
         Vector2(m_planetOrbit->GetPosition().X, 
                 m_planetOrbit->GetPosition().Y) + g_orbitPointVec);
   if(m_planetOrbit != NULL)
      g_a->SetPosition( targetPos );
   theWorld.Add(g_a,10);

   switch(g_moveState)
   {      
   case PlanetOrbit:
      {
         assert(m_planetOrbit != NULL);
         float angle = PIOver4 * dt * m_speed / m_planetOrbit->GetSize().X;
         g_orbitPointVec = Vector2::Rotate(g_orbitPointVec, angle);
         MoveInDirection(targetPos - m_head->GetPosition(), m_speed * dt);
      }
      break;
   case PlanetClick: 
      {
         MoveInDirection(targetPos - m_head->GetPosition(), m_speed * dt);
         if(m_planetOrbit->GetPosition().X - GetPosition().X <= 0.1 &&
            m_planetOrbit->GetPosition().Y - GetPosition().Y <= 0.1 )
            g_moveState = PlanetOrbit;
      }
      break;
   case Mouse:
      {
         MoveInDirection(MathUtil::ScreenToWorld(g_screenCoords) - 
                            m_head->GetPosition(),
                         m_speed * dt);
      }
      break;
   case NotMoving: break;
   default: assert(!"shouldn't get here");
   }
}

void Worm::SetPlanetOrbit(Planet* planet)
{
   m_planetOrbit = planet;

   g_orbitPointVec = Vector2(m_planetOrbit == NULL ? ORBIT_DISTANCE : planet->GetSize().X, 0);
   g_moveState = m_planetOrbit == NULL ? Mouse : PlanetClick;
}
void Worm::MouseMotionEvent(Vec2i screenCoords)
{
   g_screenCoords = screenCoords;
}

void Worm::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
}

void Worm::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
   if(m_planetOrbit == NULL)
   {      
      g_moveState = NotMoving;
   }
}

bool Worm::IntersectsWithRect( const struct Rect<float>& theirRect) const
{
   const Vector2& pos = m_head->GetPosition();
   const Vector2& sz = m_head->GetSize();
   Point<float> ul(pos.X - sz.X * 0.5f, pos.Y - sz.Y * 0.5f);
   Point<float> lr(pos.X + sz.X * 0.5f, pos.Y + sz.Y * 0.5f);
   Rect<float> headRect(ul,lr);
   if(RectanglesIntersect(theirRect, headRect))
      return true;
   if(m_tail->IntersectsWithRect(theirRect))
      return true;

   return false;
}