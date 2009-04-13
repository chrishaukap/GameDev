#ifndef CHARACTERBASE_H
#define CHARACTERBASE_H

#include "actor/gameObject.h"

#include "math/vector3d.h"

namespace CDH
{

class Projectile;
class ForegroundObject;
class CharacterBase : public GameObject
{
public:
   CharacterBase(	const Vector3& position, 
						const Vector3& rotation, 
						const Rectangle& polygon, 
						CollidableType type,
						CHUint team);  
   virtual ~CharacterBase();

	// inherited from GameObject
   virtual Status render() const;
	virtual void resolveCollision( const Collidable& object );

	CHUint team() const {return m_team;}
   const Vector3& oldPosition() const {return m_oldPosition;}

protected:
   Vector3 m_oldPosition;
	CHUint m_team;

   // derived classes should implement this to specify what happens at collision
   virtual void resolveCollisionWithProjectile( const Projectile& object ) = 0;
   virtual void resolveCollisionWithEnemy( const CharacterBase& object ) = 0;
	virtual void resolveCollisionWithTerrain( const ForegroundObject& object ) = 0;

private:
   CharacterBase(const CharacterBase& C);
   const CharacterBase& operator=(const CharacterBase& C);
};


}
#endif //CHARACTERBASE_H
