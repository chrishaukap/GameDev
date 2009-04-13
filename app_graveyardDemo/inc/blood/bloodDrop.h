#ifndef BLOODDROP_H
#define BLOODDROP_H

#include "vector3d.h"

class BloodDrop
{
public:
   BloodDrop(const Vector3& position,
             const Vector3& orientation,
             float speed,
             float size,
             float duration) :
      position(position), 
      orientation(orientation),
      speed(speed), 
      size(size),
      duration(0),
      maxDuration(duration)
      {}
   ~BloodDrop(){}

   void update();
   bool expired() const {return duration >= maxDuration;}

   // members
   Vector3 position;
   Vector3 orientation;
   float speed;
   float size;
   float duration;
   float maxDuration;
};
#endif // BLOODDROP_H