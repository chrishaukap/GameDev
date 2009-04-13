#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "defs.h"

namespace Archnoid
{

typedef enum 
{
   ShapeRect,
   ShapeCircle
} ShapeType;


namespace GameAreaDefaults
{
   const USHORT ULX = 200;
   const USHORT ULY = 200;
   const USHORT WIDTH = 1200;
   const USHORT HEIGHT = 800;
};
namespace BallDefaults
{
   const USHORT RADIUS = 6;
   const float SPEED = 1.0F;
   const USHORT X = GameAreaDefaults::ULX + GameAreaDefaults::WIDTH / 2;
   const USHORT Y = GameAreaDefaults::ULY + GameAreaDefaults::HEIGHT - 20;
   const SHORT VECTOR_X = 5;
   const SHORT VECTOR_Y = -3;
};

namespace BlockDefaults
{
   const ShapeType SHAPE = ShapeRect;
   const USHORT WIDTH = 20;
   const USHORT HEIGHT = 20;
   const USHORT HP = 1;
   const float SPEED = 1.0F;
   const SHORT VECTOR_X = 0;
   const SHORT VECTOR_Y = 0;
};

namespace BlockCollectionDefaults
{
   const USHORT NUMBLOCKS = 160;   
};

namespace GamePlayDefaults
{
   const USHORT NUMBALLS = 2; // HAUKAP - not hooked up yet
   const USHORT RESPAWN_RATE = 15;
   const USHORT SLEEPTIME = 10;
};

} // end namespace

#endif // DEFAULTS_H