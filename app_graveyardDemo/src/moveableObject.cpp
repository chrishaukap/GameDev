#include "moveableObject.h"

MoveableObject::MoveableObject(const Vector3& position, const Vector3& rotation):
   Particle(),
   m_rotation(rotation) 
{
   setPosition(position);
   m_rotation.normalise();
}