#include "stdafx.h"
#include "Alien.h"


Alien::Alien() : 
   m_wormeSpentMoving(0),
   m_wormeToSpendMoving(0),
   m_orientation(0,0),
   m_speed(0)
{
   SetColor(1,0,0,1);
   SetSize(1.5);
   SetPosition(-6,-6);
}

void Alien::Update(float dt)
{
   m_wormeSpentMoving += dt;
   if(m_wormeSpentMoving > m_wormeToSpendMoving)
   {
      m_wormeSpentMoving = 0;
      m_wormeToSpendMoving = MathUtil::RandomFloatInRange( 1.5, 5);
      m_orientation = MathUtil::RandomVector(Vector2(1,1));
      m_orientation.Normalize();
   }

   _position += m_orientation * dt * m_speed;

}
