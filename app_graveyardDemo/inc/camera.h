#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"
#include "vector3D.h"

class Vector3;
struct Camera
{
   static const float yMax;
   static const float yMin;
   Camera(const Vector3& v = CAMERA_LOCATION);

   void up();
   void down();
   void zoomIn();
   void zoomOut();
   void moveLateralDistance( float distXAxis );
   void process();

   float dist, x,y,z, yaw, spd;
   bool orbiting;
   bool yGoingUp, yOscillating;
private:
   void computePosition();
};

#endif 

