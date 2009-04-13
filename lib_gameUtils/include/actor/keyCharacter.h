#ifndef KEYCHARACTER_H
#define KEYCHARACTER_H

#include "characterBase.h"

namespace CDH
{

class KeyInput;
struct Rectangle;
class Projectile;
class ForegroundObject;
class KeyCharacter : public CharacterBase
{
public:
   KeyCharacter(const KeyInput& input, 
					 const Vector3& position, 
					 const Vector3& rotation, 
					 const Rectangle& polygon);  
   virtual ~KeyCharacter();

	// inherited from CharacterBase
   Status process();
   
protected:   
   const KeyInput& m_input;
   static const CHUint KeyCharacter::m_playerTeam;

   void resolveCollisionWithProjectile( const Projectile& object );
   void resolveCollisionWithEnemy( const CharacterBase& object );
	void resolveCollisionWithTerrain( const ForegroundObject& object );

private:
   KeyCharacter(const KeyCharacter& C);
   const KeyCharacter& operator=(const KeyCharacter& C);
};


}
#endif //KEYCHARACTER_H
