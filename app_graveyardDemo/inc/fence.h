#ifndef FENCE_H
#define FENCE_H

#include "vector3d.h"

class FenceSegment
{
public:
   FenceSegment(const Vector3& point, float angle, float scale );
   ~FenceSegment();

   float getX() const;
   float getY() const;
   float getZ() const;
   void setX(float x);
   void setY(float y);
   void setZ(float z);
   float getAngle() const;
   void setAngle(float angle);
   float getScale() const;
   void setScale(float scale);

   void render() const;
private:
   FenceSegment();   
   FenceSegment(const FenceSegment& T);
   const FenceSegment& operator=(const FenceSegment& T);

   Vector3 m_point;
   float m_fenceAngle;
   float* m_spikeAngles;
   float m_scale;
   
   static const int NumSpikes;  
};
#endif // FENCE_H