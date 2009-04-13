#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "cdhBase.h"

namespace CDH
{

class SpatialObject;
class Collidable
{
public:
	enum CollidableType
	{
		CollidableCharacter = 0,
		CollidableProjectile,
		CollidableTerrain,
		InvalidCollidableType
	};
	Collidable(CollidableType type, const SpatialObject& object );
	virtual ~Collidable(){}

	virtual bool collidedWith( const Collidable& object ) const;
	virtual void resolveCollision( const Collidable& object );

	const SpatialObject& spatialObject() const { return m_spatialObject; }
	const CollidableType& collidableType() const { return m_collidableType; }

protected:
	CollidableType m_collidableType;
	const SpatialObject& m_spatialObject;

private:
   Collidable(const Collidable& C);
   const Collidable& operator=(const Collidable& C);
};

}

#endif // COLLIDABLE_H

