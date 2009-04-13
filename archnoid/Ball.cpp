#include "stdafx.h"
#include "Ball.h"
#include <assert.h>

namespace Archnoid
{

void Ball::Move()
{
   m_cntrX += (SHORT)(m_vectX * m_speed);
   m_cntrY += (SHORT)(m_vectY * m_speed);
}
void Ball::UnMove()
{
   m_cntrX -= (SHORT)(m_vectX * m_speed);
   m_cntrY -= (SHORT)(m_vectY * m_speed);   
}
void Ball::ResolveCollision( SurfaceType surface )
{
   switch(surface)
   {
   case Vertical: 
      m_vectX *= -1; 
      break;
   case Horizontal:
      m_vectY *= -1; 
      break;
   default: 
      assert(!"invalid surface"); 
      break;
   };
   /*
   USHORT rnd = rand()%6;
   switch(rnd)
   {}
   if( (int)rand()%6 == 0 )
   {

      if( m_vectX > 10 ) --m_vectX;
      else if( m_vectX < -10 ) ++m_vectX;
      else if( m_vectX >= 0 ) ++m_vectX;
      else --m_vectX;

      if( m_vectY > 10 ) --m_vectY;
      else if( m_vectY < -10 ) ++m_vectY;
      else if( m_vectY >= 0 ) ++m_vectY;
      else --m_vectY;
   }
   else*/
}

Ball* Balls::GetFirstBall() const
{
   if( m_balls.size() )
   {
      m_ballIter = 0;
      return m_balls[m_ballIter];
   }
   else
      return NULL;
}
Ball* Balls::GetNextBall() const
{
   if( m_balls.size() )
   {
      if( ++m_ballIter == m_balls.size() )
         return NULL;
      else
         return m_balls[m_ballIter];
   }
   return NULL;

}


} // end namespace