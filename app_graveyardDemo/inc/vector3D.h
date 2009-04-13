#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "defines.h"

class Vector3
{
public: 
   float x;
   float y;
   float z;
private:    
   float pad;
public:
   Vector3();
   Vector3(const float x, const float y, const float z);
   Vector3(const Vector3& v);

   float operator[](unsigned i) const;
   float& operator[](unsigned i) ;
   void operator+=(const Vector3& v);
   Vector3 operator+(const Vector3& v) const;
   void operator-=(const Vector3& v);
   Vector3 operator-(const Vector3& v) const;
   void operator*=(const float value);
   Vector3 operator*(const float value) const;

   /**
   * Calculates and returns a component-wise product of this
   * vector with the given vector.
   */
   Vector3 componentProduct(const Vector3 &vector) const;

   /**
   * Performs a component-wise product with the given vector and
   * sets this vector to its result.
   */
   void componentProductUpdate(const Vector3 &vector);

   /**
   * Calculates and returns the vector product of this vector
   * with the given vector.
   */
   Vector3 vectorProduct(const Vector3 &vector) const;

   /**
   * Updates this vector to be the vector product of its current
   * value and the given vector.
   */
   void operator %=(const Vector3 &vector);

   /**
   * Calculates and returns the vector product of this vector
   * with the given vector.
   */
   Vector3 operator%(const Vector3 &vector) const;

   /**
   * Calculates and returns the scalar product of this vector
   * with the given vector.
   */
   float scalarProduct(const Vector3 &vector) const;

   /**
   * Calculates and returns the scalar product of this vector
   * with the given vector.
   */
   float operator *(const Vector3 &vector) const;

   float magnitude() const;
   float squareMagnitude() const;
   bool isNormalised() const;
   void normalise();
   bool operator==(const Vector3& other) const;
   bool operator!=(const Vector3& other) const;

   /**
   * Checks if this vector is component-by-component less than
   * the other.
   *
   * @note This does not behave like a single-value comparison:
   * !(a < b) does not imply (b >= a).
   */
   bool operator<(const Vector3& other) const;

   /**
   * Checks if this vector is component-by-component less than
   * the other.
   *
   * @note This does not behave like a single-value comparison:
   * !(a < b) does not imply (b >= a).
   */
   bool operator>(const Vector3& other) const;

   /**
   * Checks if this vector is component-by-component less than
   * the other.
   *
   * @note This does not behave like a single-value comparison:
   * !(a <= b) does not imply (b > a).
   */
   bool operator<=(const Vector3& other) const;

   /**
   * Checks if this vector is component-by-component less than
   * the other.
   *
   * @note This does not behave like a single-value comparison:
   * !(a <= b) does not imply (b > a).
   */
   bool operator>=(const Vector3& other) const;
   void clear() ;
   void invert();

   
  void addScaledVector(const Vector3& vector, float scale)
  {
      x += vector.x * scale;
      y += vector.y * scale;
      z += vector.z * scale;
  }

};    

class utils3D
{
private:
   utils3D();

public:   
   static float getAngleP1ToP2(float p1x, float p1y, float p2x, float p2y);
   static void rotateVecByQuat(Vector3& vec, const Vector3& axis, const float angle);
   static float distanceP1P2( const Vector3& p1, const Vector3& p2 );   
   static Vector3 translatePointAlongVector(const Vector3& p, const Vector3& v, float len);

   // returns a vector from p1 to p2
   static Vector3 
      getVectorP1ToP2(const Vector3& p1, const Vector3& p2);
   // returns unit vector from p1 to p2
   static Vector3 
      getUnitVectorP1ToP2(const Vector3& p1, const Vector3& p2);

   static float randomizeValue( float angle, float fudge );
   
   // randomize a vector by applying a random amount
   // of the total "fudge" factor to each axis of the vector
   static Vector3 
      randomizeVec(const Vector3& vec, float fudge);
   
   // randomize a normalized vector (orientation vector) by applying a random amount
   // of the total "fudge" factor to each axis of the vector
   // returns a normalized vector
   static Vector3 
      randomizeUnitVec(const Vector3& vec, float fudge);

   // returns a randomized vector from p1 to p2
   static Vector3 
      getRandomizedVectorP1ToP2( const Vector3& p1, 
                                 const Vector3& p2, 
                                 float fudge);
   
   // returns a randomized & normalized vector from p1 to p2
   static Vector3 
      getRandomizedUnitVectorP1ToP2(const Vector3& p1, 
                                    const Vector3& p2, 
                                    float fudge);

};

#endif // VECTOR3D_H