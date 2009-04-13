#include "actor/spatialObject.h"


using namespace CDH;

SpatialObject::SpatialObject(const Vector3& position,
									  const Vector3& rotation, 
									  const Rectangle& polygon ) :
   Particle(),
   m_rotation(rotation),
	m_polygon(polygon),
	m_toBeDeleted(false)
{
   setPosition(position);
   m_rotation.normalise();
}
	
void 
SpatialObject::scaleObject(float scaleFactor)
{
	m_polygon.width *= scaleFactor;
	m_polygon.height *= scaleFactor;
}