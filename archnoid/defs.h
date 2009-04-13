#ifndef DEFS_H
#define DEFS_H

namespace Archnoid
{

// typedefs
typedef unsigned short USHORT;
typedef short SHORT;

// collision
const USHORT NO_COLLISION = 0;

typedef enum
{
   None,
   Northern,
   Southern,
   Western,
   Eastern   
} CollisionType;

typedef enum
{
   Vertical = 1,
   Horizontal
} SurfaceType;


} // end namespace


#endif // DEFS_H