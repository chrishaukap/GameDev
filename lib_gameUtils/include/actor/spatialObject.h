#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H

#include "physics/particle.h"

namespace CDH
{

struct Rectangle
{
	Rectangle(float w, float h) : width(w), height(h) {}
	float width;
	float height;
};

class SpatialObject : public Particle
{
public:
   SpatialObject(const Vector3& position, const Vector3& rotation, const Rectangle& polygon);
   virtual ~SpatialObject(){}

   virtual void setRotation(const Vector3& rotation) {m_rotation = rotation;}
   virtual const Vector3& rotation() const {return m_rotation;}
	const Rectangle& polygon() const {return m_polygon;}

	void scaleObject(float scaleFactor);

	// HAUKAP - move to game object
	void markForRemoval() {m_toBeDeleted = true;}
   bool isMarkedForRemoval() const {return m_toBeDeleted;}
   
   // DEBUG!
   void unmark() {m_toBeDeleted = false;}

   // HAUKAP - todo - rotate by quaternions 

protected:
   Vector3 m_rotation;
	Rectangle m_polygon;
	bool m_toBeDeleted;
};
}

#endif // SPATIALOBJECT_H