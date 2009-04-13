#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3d.h"
    
namespace CDH
{

class Quaternion
{
public:
   union {
      struct {
         float r; //real element 
         float x;
         float y;
         float z;
      };

      float data[4];
   };

   Quaternion() ;   // creates a quaternion with zero rotation
   Quaternion(const float r, const float x, const float y, const float z) ;
   void normalise();


   Quaternion operator* (const Quaternion& q) const;
   Quaternion operator* (float fScalar) const;

   void operator *=(const Quaternion &multiplier);

   //! returns the euler angles from a rotation quaternion
	Vector3 toEulerAngles() const;

	void to_axis_angle(Vector3 &axis, float &angle) const;

};

}
#endif 