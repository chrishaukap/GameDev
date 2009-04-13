#include "StdAfx.h"
#include "Block.h"
#include "Ball.h"

namespace Archnoid
{

Block::Block(USHORT x, USHORT y, USHORT w, USHORT h, 
             USHORT hp, float speed):
   m_x(x),
   m_y(y),
   m_width(w),
   m_height(h),
   m_hitpoints(hp),
   m_destroyed(false),
   m_speed(speed),
   m_vectX(0),
   m_vectY(0)
{
}

Block::~Block(void)
{
}

USHORT Block::DecrementHP()
{
   if(m_hitpoints > 0)
      --m_hitpoints;

   if(m_hitpoints ==0)
      m_destroyed = true;

   return m_hitpoints;
}

void Block::Move()
{
   m_x += (SHORT)(m_vectX * m_speed);
   m_y += (SHORT)(m_vectY * m_speed);
}

CollisionType Block::IsCollision(const Ball &ball) const
{
   CollisionType collision = None;
   bool xShared = false, yShared = false;

   // see if there's a collision through bounds testing
   // then determine the direction of the collision

   if( // check the horizontal 
       ball.GetCntrX() >= m_x &&
         ball.GetCntrX() <= m_x + m_width)
   {
      xShared = true;
   }
   if( // check the vertical
       ball.GetCntrY() >= m_y &&
         ball.GetCntrY() <= m_y + m_height)
   {
      yShared = true;
   }

   if(xShared && yShared)
   {
      // test the vector, return appropriate collision
      if( ball.GetVectX() < 0 )
         return Western;
      else
         return Eastern;
      if( ball.GetVectY() < 0 )
         return Southern;
      else
         return Northern;   
   }
   return collision ;
}

} // end namespace