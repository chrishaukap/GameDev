#include "stdafx.h"
#include "GameArea.h"
#include "Ball.h"
#include "defs.h"

namespace Archnoid
{


CollisionType GameArea::OutOfBounds(const Ball& ball) const
{
   CollisionType collision = None;
   if( ball.GetULY() < m_north )
   {
      collision = Northern;
   }
   else if( ball.GetLRY() > m_south )
   {
      collision = Southern;
   }
   else if( ball.GetULX() < m_west)
   {
      collision = Western;
   }
   else if( ball.GetLRX() > m_east )
   {
      collision = Eastern;
   }
   return collision;
}

void GameArea::Shrink(USHORT amount)
{
   m_east  -= amount;
   m_west  += amount;
   m_south -= amount;
   m_north += amount;
}

void GameArea::Grow(USHORT amount)
{
   m_east  += amount;
   m_west  -= amount;
   m_south += amount;
   m_north -= amount;
}


} // end namespace