#ifndef GRAVEYARD_H
#define GRAVEYARD_H

#include "vector3d.h"

class GraveyardBounds
{
public:
   static float backWallZ;
   static float frontWallZ;
   static float leftWallX;
   static float rightWallX;
   static Vector3 farLeft;
   static Vector3 farRight;
   static Vector3 nearLeft;
   static Vector3 nearRight;
private:
   GraveyardBounds();
};

#endif // GRAVEYARD_H

