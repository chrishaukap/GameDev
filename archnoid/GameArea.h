#ifndef GAMEAREA_H
#define GAMEAREA_H

#include "defs.h"
#include "defaults.h"
#include "Ball.h"

namespace Archnoid
{

class GameArea
{
public:
   GameArea(): m_north(NORTH_WALL),m_south(SOUTH_WALL),
               m_east(EAST_WALL),m_west(WEST_WALL){}
   ~GameArea(){}
   // returns true if collision ocurred
   CollisionType OutOfBounds(const Ball& ball) const;
   void Shrink(USHORT amount);
   void Grow(USHORT amount);
   USHORT GetNorthWall() const {return m_north;}
   USHORT GetSouthWall() const {return m_south;}
   USHORT GetWestWall() const {return m_west;}
   USHORT GetEastWall() const {return m_east;}

private:
   static const USHORT NORTH_WALL = GameAreaDefaults::ULY;
   static const USHORT WEST_WALL = GameAreaDefaults::ULX;
   static const USHORT EAST_WALL = GameAreaDefaults::ULX + GameAreaDefaults::WIDTH;
   static const USHORT SOUTH_WALL = GameAreaDefaults::ULY + GameAreaDefaults::HEIGHT;
   USHORT m_north,m_south,m_west,m_east;
};


} // end namespace

#endif // GAMEAREA_H