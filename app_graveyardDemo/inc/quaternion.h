#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3d.h"
    
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

	void to_axis_angle(Vector3 &axis, float &angle) const
	{
		angle = acosf(r);

		// pre-compute to save time
		float sinf_theta_inv = 1.0/sinf(angle);

		// now the vector
		axis.x = x * sinf_theta_inv;
		axis.y = y * sinf_theta_inv;
		axis.z = z * sinf_theta_inv;

		// multiply by 2
		angle *= 2;
	}

};

#endif 