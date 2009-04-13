#include "actor/collidable.h"
#include "actor/spatialObject.h"
#include "math/vector3D.h"

using namespace CDH;

Collidable::Collidable(CollidableType type, const SpatialObject& object ) :
	m_collidableType(type),
	m_spatialObject(object)
{}
		
bool Collidable::collidedWith( const Collidable& object ) const
{
	bool collision = false;
	
	return utils3D::RectToRectCollision2D( 
		m_spatialObject.position(), 
		Vector3(m_spatialObject.position().x + m_spatialObject.polygon().width,
				  m_spatialObject.position().y + m_spatialObject.polygon().height,
				  0.0f),
		object.spatialObject().position(), 
		Vector3(object.spatialObject().position().x + object.spatialObject().polygon().width,
				  object.spatialObject().position().y + object.spatialObject().polygon().height,
				  0.0f));
}

void 
Collidable::resolveCollision( const Collidable& object )
{
	// default implementation is to do nothing
	return;
}
