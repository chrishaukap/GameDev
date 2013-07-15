#pragma once

#include "iCollidable.h"

class Planet;
class WormHead;
class WormTail;
class Worm : public MouseListener, public iCollidable
{
public:
	Worm();
	~Worm();

   void Init();
   void Clear();
	void Update(float dt);
	
	const Vector2 GetPosition();
	const Vector2 GetSize();
   bool IntersectsWithRect( const struct Rect<float>& ) const;

   void AddToWorld();
   void GrowTail();
   void SetPlanetOrbit(Planet* planet);

   // inherited from MouseListener
   void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
   void MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button);
   void MouseMotionEvent(Vec2i screenCoordinates);

   IMPLEMENT_INDEXABLE_CLASS(Worm)
private:
   void MoveInDirection(Vector2& direction, float distance);
   WormHead* m_head;
   WormTail* m_tail;
   float m_speed;
   Planet* m_planetOrbit;
};