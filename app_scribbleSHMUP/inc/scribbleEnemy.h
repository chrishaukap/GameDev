#ifndef SCRIBBLEENEMY_H
#define SCRIBBLEENEMY_H

#include "actor/AICharacter.h"

namespace CDH
{

struct Rectangle;
class Projectile;
class Vector3;
class ScribbleEnemy : public AICharacter
{
public:
   ScribbleEnemy(const Vector3& position, 
					  const Vector3& rotation, 
					  const Rectangle& polygon,
					  CHUint hitpoints,
                 bool isBoss = false);  
   virtual ~ScribbleEnemy();
      
	virtual Status process();

   static void setDeathSound(CHUlong sound) {m_deathSound = sound;}
   static void setSpawnSound(CHUlong sound) {m_spawnSound = sound;}
   static void setHitSound(CHUlong sound) {m_hitSound = sound;}

   bool isBoss() const {return m_isBoss;}

public:   
   static CHUlong m_deathSound;
   static CHUlong m_spawnSound;
   static CHUlong m_hitSound;
   bool m_isBoss;
	
private:
   // inherited from CharacterBase
   void resolveCollisionWithProjectile( const Projectile& object );
   void resolveCollisionWithEnemy( const CharacterBase& object );
	void resolveCollisionWithTerrain( const ForegroundObject& object );

   ScribbleEnemy(const ScribbleEnemy& C);
   const ScribbleEnemy& operator=(const ScribbleEnemy& C);
};



class ScribbleBoss : public ScribbleEnemy
{
public:
   ScribbleBoss(const Vector3& position, 
					const Vector3& rotation, 
					const Rectangle& polygon,
					CHUint hitpoints);  
   virtual ~ScribbleBoss();

	Status process();

private:
   // inherited from CharacterBase
   void resolveCollisionWithEnemy( const CharacterBase& object );
   void resolveCollisionWithProjectile( const Projectile& object );
	void shrink();
	
   ScribbleBoss(const ScribbleBoss& C);
   const ScribbleBoss& operator=(const ScribbleBoss& C);
};

class GameEngine;
class Level1Boss : public ScribbleEnemy
{
public:
   Level1Boss(const Vector3& position, 
				  const Vector3& rotation, 
				  const Rectangle& polygon,
              GameEngine& engine);  
   virtual ~Level1Boss();

	Status process();
   CHUint hitpoints() const {return m_hitpoints;}  // HAUKAP - further up the hierarcy?

private:   
	GameEngine& m_engine;
	float m_jumpVelocity;

   // inherited from CharacterBase
   void resolveCollisionWithEnemy( const CharacterBase& object );
   void resolveCollisionWithProjectile( const Projectile& object );

   void jump();
   void fire();
	
   Level1Boss();
   Level1Boss(const Level1Boss& C);
   const Level1Boss& operator=(const Level1Boss& C);
};


}
#endif //SCRIBBLEENEMY_H
