#include "cdhBase.h"

#include "actor/gameObject.h"
#include "actor/projectile.h"
#include "actor/foregroundObject.h"
#include "actor/characterBase.h"
//#include "sprite/sprite.h"
//#include "math/vector3D.h"
#include <assert.h>

#pragma warning(disable:4355) //'this' : used in base member initializer list

using namespace CDH;

GameObject::GameObject(const Vector3& position, 
							  const Vector3& rotation, 
							  const Rectangle& polygon,
							  CollidableType type) : 
   SpatialObject(position, rotation, polygon),
	Collidable(type, *this),
   m_oldPosition(0.0f, 0.0f, 0.0f),
	m_facing(FacingRight),
   m_isEnemy(false)
{
		
}
GameObject::~GameObject()
{}

