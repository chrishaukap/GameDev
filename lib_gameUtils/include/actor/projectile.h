#ifndef PROJECTILE_h
#define PROJECTILE_h

#include "actor/gameObject.h"

namespace CDH
{

class KeyInput;
struct Rectangle;
class Projectile : public GameObject
{
public:	
   Projectile(const Vector3& position, 
				  const Vector3& rotation, 
				  const Vector3& orientation,
				  float velocity,
				  float mass,
				  const Rectangle& polygon,
				  CHUint team);  
   virtual ~Projectile();

   const CHUint team() const {return m_team;}

	// inherited from GameObject
   Status process();
	Status render() const;
	void resolveCollision( const Collidable& object );

	   
private:
	CHUint m_team;  // who owns the bullet
   CHUint m_lifetime;

   Projectile(const Projectile& C);
   const Projectile& operator=(const Projectile& C);
};


}
#endif //PROJECTILE_h
