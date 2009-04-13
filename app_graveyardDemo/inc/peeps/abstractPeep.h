#ifndef ABSTRACTPEEP_H
#define ABSTRACTPEEP_H

#include "vector3D.h"
#include "../moveableObject.h"

enum PeepType
{
   TypePeepHead,
   TypePeepBody,
   TypePeep,
   TypeDeadPeep,  //??
};

class AbstractPeep : public MoveableObject
{
protected:
   enum PeepState   
   {
      PeepFree,
      PeepTargeted,
      PeepThrashing,
      PeepFinishedThrashing
   };
public:
   AbstractPeep(const Vector3& position, const Vector3& orientation, const Vector3& rotation );
   virtual ~AbstractPeep(){}

   virtual void render() const = 0;
   virtual void renderDead() const = 0;
   
   virtual void setOrientation(const Vector3& orientation) { m_orientation = orientation; }
   virtual const Vector3& orientation() const { return m_orientation; }

   virtual PeepType peepType() const =0;
   
   // inherited from Moveableobject
   virtual void setFinishedThrash(const Vector3& endVelocity) ;
   virtual void setTargeted() {m_state = PeepTargeted;}
   virtual void setThrashing() {m_state = PeepThrashing;}
   virtual bool finishedThrashing() const {return m_state == PeepFinishedThrashing;}
   virtual bool isFree() const {return m_state == PeepFree;}
   bool isThrashing() const {return m_state == PeepThrashing;}
   virtual BoundingCube getBoundingCube() const = 0;

protected:
   Vector3 m_orientation;
   Vector3 m_rotation;
   PeepState m_state;
};


#endif // ABSTRACTPEEP_H