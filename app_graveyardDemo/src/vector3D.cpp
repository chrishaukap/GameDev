#include "defines.h"
#include "vector3D.h"
#include "quaternion.h"
#include <math.h>
#include "assert.h"


Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}
Vector3::Vector3(const Vector3& v) {x = v.x; y = v.y; z = v.z;}

float Vector3::operator[](unsigned i) const
{
   if (i == 0) return x;
   if (i == 1) return y;
   return z;
}
float& Vector3::operator[](unsigned i) 
{
   if (i == 0) return x;
   if (i == 1) return y;
   return z;
}

void Vector3::operator+=(const Vector3& v)
{
   x += v.x;
   y += v.y;
   z += v.z;
}
Vector3 Vector3::operator+(const Vector3& v) const
{
   return Vector3(x+v.x, y+v.y, z+v.z);
}
void Vector3::operator-=(const Vector3& v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
}
Vector3 Vector3::operator-(const Vector3& v) const
{
   return Vector3(x-v.x, y-v.y, z-v.z);
}
void Vector3::operator*=(const float value)
{
   x *= value;
   y *= value;
   z *= value;
}
Vector3 Vector3::operator*(const float value) const
{
   return Vector3(x*value, y*value, z*value);
}
/**
* Calculates and returns a component-wise product of this
* vector with the given vector.
*/
Vector3 Vector3::componentProduct(const Vector3 &vector) const
{
   return Vector3(x * vector.x, y * vector.y, z * vector.z);
}

/**
* Performs a component-wise product with the given vector and
* sets this vector to its result.
*/
void Vector3::componentProductUpdate(const Vector3 &vector)
{
   x *= vector.x;
   y *= vector.y;
   z *= vector.z;
}

/**
* Calculates and returns the vector product of this vector
* with the given vector.
*/
Vector3 Vector3::vectorProduct(const Vector3 &vector) const
{
   return Vector3(y * vector.z - z * vector.y, 
                  z * vector.x - x * vector.z, 
                  x * vector.y - y * vector.x);
}
/**
* Updates this vector to be the vector product of its current
* value and the given vector.
*/
void Vector3::operator %=(const Vector3 &vector)
{
   *this = vectorProduct(vector);
}

/**
* Calculates and returns the vector product of this vector
* with the given vector.
*/
Vector3 Vector3::operator%(const Vector3 &vector) const
{
   return Vector3(y*vector.z-z*vector.y, 
                  z*vector.x-x*vector.z, 
                  x*vector.y-y*vector.x);
}

/**
* Calculates and returns the scalar product of this vector
* with the given vector.
*/
float Vector3::scalarProduct(const Vector3 &vector) const
{
   return x*vector.x + y*vector.y + z*vector.z;
}
/**
* Calculates and returns the scalar product of this vector
* with the given vector.
*/
float Vector3::operator *(const Vector3 &vector) const
{
   return x*vector.x + y*vector.y + z*vector.z;
}       

float Vector3::magnitude() const
{
   return sqrt(x*x+y*y+z*z);
}
float Vector3::squareMagnitude() const
{
   return x*x+y*y+z*z;
}
bool Vector3::isNormalised() const
{
   const float eps = 0.000001f;
   float mag = magnitude();
   float magEps = mag - 1.0f;
   return magEps <= eps ? true: false;
}
void Vector3::normalise()
{
   float l = magnitude();
   if (l > 0) 
   {
      (*this) *= ((float)1)/l;
   }
}
bool Vector3::operator==(const Vector3& other) const
{
   return x == other.x &&
          y == other.y &&
          z == other.z;
}
bool Vector3::operator!=(const Vector3& other) const
{
   return !(*this == other);
}

/**
* Checks if this vector is component-by-component less than
* the other.
*
* @note This does not behave like a single-value comparison:
* !(a < b) does not imply (b >= a).
*/
bool Vector3::operator<(const Vector3& other) const
{
   return x < other.x && y < other.y && z < other.z;
}

/**
* Checks if this vector is component-by-component less than
* the other.
*
* @note This does not behave like a single-value comparison:
* !(a < b) does not imply (b >= a).
*/
bool Vector3::operator>(const Vector3& other) const
{
   return x > other.x && y > other.y && z > other.z;
}

/**
* Checks if this vector is component-by-component less than
* the other.
*
* @note This does not behave like a single-value comparison:
* !(a <= b) does not imply (b > a).
*/
bool Vector3::operator<=(const Vector3& other) const
{
   return x <= other.x && y <= other.y && z <= other.z;
}

/**
* Checks if this vector is component-by-component less than
* the other.
*
* @note This does not behave like a single-value comparison:
* !(a <= b) does not imply (b > a).
*/
bool Vector3::operator>=(const Vector3& other) const
{
   return x >= other.x && y >= other.y && z >= other.z;
}
void Vector3::clear() 
{ 
   x = y = z = 0;
}
void Vector3::invert()
{
   x = -x;
   y = -y;
   z = -z;
}

float utils3D::getAngleP1ToP2(float p1x, float p1y, float p2x, float p2y)
{
   float dY = p1y - p2y;
   float dX = p1x - p2x;

   if( dX == 0 )
   {
      dX = (float)0.0001;
   }

   float angle = atan( dY/dX );
   return angle;
}

void 
utils3D::rotateVecByQuat(Vector3& vec, const Vector3& axis, const float angle)
{
   float mag = vec.magnitude();
   vec.normalise();

   Quaternion vq(0, vec.x, vec.y, vec.z);
   Quaternion q;

	// HAUKAP - put this in quaternion class as another constructor
   Quaternion rot( cos(angle/2), axis.x * sin(angle/2), axis.y * sin(angle/2), axis.z * sin(angle/2) );
   q = rot * q;
   q.normalise();

   Quaternion qp(q.r, -q.x, -q.y, -q.z);
   Quaternion vp = qp * vq * q;

   vec.x = vp.x * mag;
   vec.y = vp.y * mag;
   vec.z = vp.z * mag;
   return;
}

float utils3D::distanceP1P2( const Vector3& p1, const Vector3& p2 )
{
   float dx = p2.x - p1.x;
   float dy = p2.y - p1.y;
   float dz = p2.z - p1.z;
   return sqrt( dx*dx + dy*dy + dz*dz );   
}

float utils3D::randomizeValue( float value, float fudge )
{
   // amount of total fudge factor
   double randPercent = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
   float dAngle = (float)randPercent * fudge;
   
   // + or - fudge factor
   int plusOrMinus = 0;
   plusOrMinus = rand()%2;
   if(plusOrMinus == 0)
      return value + dAngle;
   else
      return value - dAngle;
}

Vector3 utils3D::translatePointAlongVector(const Vector3& p, const Vector3& v, float len)
{
   return Vector3(p.x + v.x * len, 
                  p.y + v.y * len,
                  p.z + v.z * len);
}

Vector3 utils3D::randomizeVec(const Vector3& vec, float fudge)
{
   Vector3 newVec(randomizeValue( vec.x, fudge ),
                  randomizeValue( vec.y, fudge ),
                  randomizeValue( vec.z, fudge ));
   return newVec;
}

Vector3 utils3D::randomizeUnitVec(const Vector3& vec, float fudge)
{
   assert(vec.isNormalised());

   Vector3 newVec(randomizeValue( vec.x, fudge ),
                  randomizeValue( vec.y, fudge ),
                  randomizeValue( vec.z, fudge ));
   newVec.normalise();
   return newVec;
}

Vector3 utils3D::getVectorP1ToP2(const Vector3& p1, const Vector3& p2)
{
   Vector3 vec(p2 - p1);
   return vec;
}

Vector3 utils3D::getUnitVectorP1ToP2(const Vector3& p1, const Vector3& p2)
{
   Vector3 vec(p2 - p1);
   vec.normalise();
   return vec;
}

Vector3 utils3D::getRandomizedVectorP1ToP2(const Vector3& p1, const Vector3& p2, float fudge)
{
   Vector3 vec(p2 - p1);
   return randomizeVec( vec, fudge );
}
Vector3 utils3D::getRandomizedUnitVectorP1ToP2(const Vector3& p1, const Vector3& p2, float fudge)
{
   Vector3 vec(p2 - p1);
   vec.normalise();
   return randomizeUnitVec( vec, fudge );
}

