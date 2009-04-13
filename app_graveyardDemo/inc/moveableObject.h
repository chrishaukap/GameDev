#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include "vector3D.h"
#include "particle.h"
#include <vector>

struct BoundingCube
{
   BoundingCube(float lowerX, float upperX,
                float lowerY, float upperY,
                float lowerZ, float upperZ):
      lX(lowerX), uX(upperX), lY(lowerY), uY(upperY), lZ(lowerZ), uZ(upperZ)
   {}
   float lX, uX, lY, uY, lZ, uZ;
};

// all objects that can be thrashed by a Thrasher Tree derive from this
class MoveableObject : public Particle
{
public:
   MoveableObject(const Vector3& position, const Vector3& rotation);
   virtual ~MoveableObject(){}

   virtual void setRotation(const Vector3& rotation) {m_rotation = rotation;}
   virtual const Vector3& rotation() const {return m_rotation;}
   virtual void setFinishedThrash(const Vector3& endVelocity) = 0;
   virtual void setTargeted() = 0;
   virtual void setThrashing() = 0;
   virtual bool finishedThrashing() const = 0;

   virtual BoundingCube getBoundingCube() const = 0;

protected:
   Vector3 m_rotation;
private:

};

#endif // MOVEABLEOBJECT_H