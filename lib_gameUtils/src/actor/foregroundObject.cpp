#include "actor/foregroundObject.h"

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <gl/gl.h>

using namespace CDH;

ForegroundObject::ForegroundObject(const Vector3& position, 
											  const Vector3& rotation, 
											  const CDH::Rectangle& polygon,
											  TerrainType terrainType) : 
	GameObject(position, rotation, polygon, CollidableTerrain),
	m_type(terrainType)
{
		
}
ForegroundObject::~ForegroundObject()
{}


Status 
ForegroundObject::process()
{
	return StatusSuccess;
}
Status 
ForegroundObject::render() const
{
	float x = position().x,
			y = position().y,
	      w = polygon().width,
	      h = polygon().height; 
	glBegin(GL_LINE_LOOP);						
		glVertex3f(x,		y,			0.0f);	
		glVertex3f(x + w,	y,			0.0f);
		glVertex3f(x + w,	y + h,	0.0f);
		glVertex3f(x,		y + h,	0.0f);
	glEnd();			

	return StatusSuccess;
}

void 
ForegroundObject::resolveCollision( const Collidable& object )
{
}