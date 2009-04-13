#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "actor/keyCharacter.h"
#include "math/vector3d.h"

namespace CDH
{

class GameEngine;
class KeyInput;
class Vector3;
struct Rectangle;
class MainCharacter : public KeyCharacter
{
public:
   MainCharacter(GameEngine& engine,
                  const KeyInput& input, 
                  const Vector3& position, 
                  const Vector3& rotation, 
                  const Rectangle& polygon);  
   virtual ~MainCharacter();

	// inherited from CharacterBase
   Status process();

	CHUint hitpoints() const {return m_hitpoints;}
	void setHitpoints(CHUint hp) {m_hitpoints = hp;}

   void gainLives(CHUint lives);
   void respawn();
   CHUint numLives() const;
   bool canRespawn() const;
   void decrementLife();


   static void setHitSound(CHUlong handle) {m_hitSound = handle;}
   static void setJumpSound(CHUlong handle) {m_jumpSound = handle;}
   static void setDeathSound(CHUlong handle) {m_deathSound = handle;}
   static void setHurtSounds(CHUlong handle1, CHUlong handle2) {m_hurt1Sound = handle1;m_hurt2Sound = handle2;}
   static void setPickupSound(CHUlong handle) {m_pickupSound = handle;}
   
   static const Vector3 StartingLocation;
private:   
	GameEngine& m_engine;
	float m_jumpVelocity;
	float m_isJumping;
	float m_moveSpeed;
	CHUint m_hitpoints;
   CHUint m_numLives;
   bool m_invincible;
   CHUint m_invincibilityCounter;
   static const CHUint LengthOfInvincibility;
   static const CHUint StartingHitPoints;

   // sound effects   
   static CHUlong m_deathSound;
   static CHUlong m_hitSound;
   static CHUlong m_jumpSound;
   static CHUlong m_hurt1Sound;
   static CHUlong m_hurt2Sound;
   static CHUlong m_pickupSound;
	
   
   CHUlong hurtSound() const;	
	void resolveCollisionWithTerrain( const ForegroundObject& object );
   void resolveCollisionWithEnemy( const CharacterBase& object );
   void resolveCollisionWithProjectile( const Projectile& object );

   MainCharacter(const MainCharacter& C);
   const MainCharacter& operator=(const MainCharacter& C);
};


}
#endif //KEYCHARACTER_H
