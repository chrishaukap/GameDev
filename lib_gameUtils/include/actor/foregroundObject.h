#ifndef FOREGROUNDOBJECT_H
#define FOREGROUNDOBJECT_H

#include "actor/gameObject.h"

namespace CDH
{

struct Rectangle;

// will be used to represent things like, platforms/ledges, elevators, ropes, fires (ouch), pits, etc.
// (perhaps also power ups?)
class ForegroundObject : public GameObject
{
public:
	enum TerrainType
	{
		Invalid = 0,
		Walkable,
		Spikes
	};

   ForegroundObject(const Vector3& position, 
						  const Vector3& rotation, 						  
						  const Rectangle& polygon,
						  TerrainType type = Invalid);  
   virtual ~ForegroundObject();

	// inherited from GameObject
   virtual Status process();
	virtual Status render() const;
	virtual void resolveCollision( const Collidable& object );

	TerrainType terrainType() const {return m_type;}

private:
	TerrainType m_type;

	ForegroundObject();
   ForegroundObject(const ForegroundObject& C);
   const ForegroundObject& operator=(const ForegroundObject& C);
};


}
#endif //PROJECTILE_h
