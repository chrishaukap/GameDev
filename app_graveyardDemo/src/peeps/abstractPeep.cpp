#include "peeps/abstractPeep.h"

AbstractPeep::AbstractPeep(const Vector3& position, const Vector3& orientation, const Vector3& rotation ) : 
   m_orientation(orientation),
   m_state(PeepFree),
   MoveableObject(position,rotation)
{
}

   
void AbstractPeep::setFinishedThrash(const Vector3& endVelocity) 
{
   // HAUKAP - keeping redundant information (orientation is the normalized velocity)
   setVelocity(endVelocity);

   Vector3 newOrientation(endVelocity);
   newOrientation.normalise();
   setOrientation( newOrientation );
   m_state = PeepFinishedThrashing;
}