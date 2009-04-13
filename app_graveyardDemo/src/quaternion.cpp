#include "quaternion.h"
#include "math.h"
#include "vector3d.h"

Quaternion::Quaternion() : r(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(const float r, const float x, const float y, const float z) :
   r(r), x(x), y(y), z(z) 
{
}

void Quaternion::normalise()
{
   float d = r*r + x*x + y*y + z*z;

   // Check for zero length quaternion, and use the no-rotation
   // quaternion in that case.
   if (d == 0) { 
      r = 1; 
      return;
   }

   d = ((float)1.0)/sqrt(d);
   r *= d;
   x *= d;
   y *= d;
   z *= d;
}


Quaternion Quaternion::operator* (const Quaternion& q) const
{
   Quaternion newQ;
   newQ.r = r * q.r - x * q.x - y * q.y - z * q.z;
   newQ.x = r * q.x + x * q.r + y * q.z - z * q.y;
   newQ.y = r * q.y + y * q.r + z * q.x - x * q.z;
   newQ.z = r * q.z + z * q.r + x * q.y - y * q.x;
   return newQ;
}
Quaternion Quaternion::operator* (float fScalar) const
{
   Quaternion newQ;
   newQ.r *= fScalar;
   newQ.x *= fScalar;
   newQ.y *= fScalar;
   newQ.z *= fScalar;
   return newQ;
}

void Quaternion::operator *=(const Quaternion &multiplier)
{
   Quaternion q = *this;
   r = q.r*multiplier.r - q.x*multiplier.x - q.y*multiplier.y - q.z*multiplier.z;
   x = q.r*multiplier.x + q.x*multiplier.r + q.y*multiplier.z - q.z*multiplier.y;
   y = q.r*multiplier.y + q.y*multiplier.r + q.z*multiplier.x - q.x*multiplier.z;
   z = q.r*multiplier.z + q.z*multiplier.r + q.x*multiplier.y - q.y*multiplier.x;
}

//! returns the euler angles from a rotation quaternion
Vector3 Quaternion::toEulerAngles() const
{
   float sqw = r * r;    
   float sqx = x * x;    
   float sqy = y * y;    
   float sqz = z * z;    

   Vector3 euler(
      atan2( 2.0f * (x * y + z * r), sqx - sqy - sqz + sqw),    		
      asin( -2.0f * (x * z - y * r)),
      atan2( 2.0f * (y * z + x * r), -sqx - sqy + sqz + sqw));
   return euler;
}
