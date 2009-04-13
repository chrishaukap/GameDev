#include "scribbleEnemy.h"

#include "cdhBase.h"
#include "defines.h"
#include "actor/foregroundObject.h"
#include "actor/projectile.h"
#include <assert.h>
#include "sprite/sprite.h"

#include "oggController.h"
#include "oggPlayer.h"
#include "gameEngine.h"

// to render boss
#ifdef _WINDOWS
#include <windows.h>
#endif
#include <gl/gl.h>

using namespace CDH;
CHUlong ScribbleEnemy::m_deathSound = 0;
CHUlong ScribbleEnemy::m_spawnSound = 0;
CHUlong ScribbleEnemy::m_hitSound = 0;

ScribbleEnemy::ScribbleEnemy(const Vector3& position, 
								    const Vector3& rotation, 
								    const CDH::Rectangle& polygon,
									 CHUint hitpoints,
                            bool isBoss) : 
	AICharacter(position, rotation, polygon, hitpoints),
   m_isBoss(isBoss)
{
   setEnemy(true);
	setDamping(0.9f);	
	setMass(1.0f);	
}
ScribbleEnemy::~ScribbleEnemy()
{}

Status 
ScribbleEnemy::process()
{
	setFacing(FacingRight);
   if( --m_lifetime  == 0 )
      markForRemoval();

   // just move left for now.
	const float speed = 0.35f;
	m_oldPosition = position();
	setPosition( 
		Vector3( m_oldPosition.x - speed, m_oldPosition.y, m_oldPosition.z));
	
	addForce(GRAVITY);
	integrate(0.25f);	

	sprite()->animate();
	return StatusSuccess;
}


void 
ScribbleEnemy::resolveCollisionWithProjectile( const Projectile& object )
{
   const float knockbackAmount = 4.0f;
   bool right = object.orientation().x > 0;
   
   Vector3 oldPosition(position());
   setPosition( oldPosition.x + (right ? knockbackAmount : -knockbackAmount),
                oldPosition.y, oldPosition.z);   
   
   int rnd = rand() % 2; 

   --m_hitpoints;
   if(m_hitpoints == 0)
   {
      markForRemoval();
      OggPlayer* sound = OggController::getOggPlayer(m_deathSound);
      if(sound != NULL)
         Status stat = sound->play();
   }
   else
   {
      if(rnd == 0)
      {
         OggPlayer* sound = OggController::getOggPlayer(m_hitSound);
         if(sound != NULL)
            Status stat = sound->play();
      }   
   }
}
void 
ScribbleEnemy::resolveCollisionWithEnemy( const CharacterBase& object )
{
	if( object.velocity().y < 0.0f && 
       object.oldPosition().y >= (position().y + polygon().height))
   {
      OggPlayer* sound = OggController::getOggPlayer(m_deathSound);
      if(sound != NULL)
         Status stat = sound->play();
      markForRemoval();
   }
}
void 
ScribbleEnemy::resolveCollisionWithTerrain( const ForegroundObject& object )
{
	if(object.terrainType() == ForegroundObject::Walkable)
	{ 
		if(velocity().y < 0.0f)
		{	
			if(m_oldPosition.y >= (object.position().y + object.polygon().height))
			{
				setVelocity(Vector3(0.0f, 0.0f, 0.0f));
				clearAccumulator();
				setPosition( position().x, 
								 object.position().y + object.polygon().height, 
								 position().z );
			}
		}
	}
}


ScribbleBoss::ScribbleBoss(const Vector3& position, 
									const Vector3& rotation, 
									const CDH::Rectangle& polygon,
									CHUint hitpoints) : 
	ScribbleEnemy(position, rotation, polygon, hitpoints)
{
	setDamping(0.9f);	
	setMass(2.0f);	
}
ScribbleBoss::~ScribbleBoss()
{}

Status 
ScribbleBoss::process()
{
   // just move left for now.
	const float speed = 0.20f;
	m_oldPosition = position();
	static bool addIt = true;
	static CHUint addItCounter = 0;
	if( ++addItCounter > 80 )
	{
		addIt = !addIt;
		addItCounter = 0;
	}
	
	setFacing( addIt ? FacingLeft : FacingRight );  // facing is backwards because of image
	setPosition( 
		Vector3( m_oldPosition.x + (addIt ? speed : -speed), 
					m_oldPosition.y, m_oldPosition.z));
	
	addForce(GRAVITY);
	integrate(0.25f);	

	if(sprite() != NULL)
		sprite()->animate();

	return StatusSuccess;
}

void
ScribbleBoss::shrink()
{
	scaleObject(0.9f);
}
void 
ScribbleBoss::resolveCollisionWithProjectile( const Projectile& object )
{
   --m_hitpoints;
	shrink();
   
   if(m_hitpoints == 0)
   {
      markForRemoval();
      OggPlayer* sound = OggController::getOggPlayer(m_deathSound);
      if(sound != NULL)
         Status stat = sound->play();
   }
   else
   {
      OggPlayer* sound = OggController::getOggPlayer(m_hitSound);
      if(sound != NULL)
         Status stat = sound->play();
   }
}
void 
ScribbleBoss::resolveCollisionWithEnemy( const CharacterBase& object )
{
	if( object.velocity().y < 0.0f && 
       object.oldPosition().y >= (position().y + polygon().height))
   {
      OggPlayer* sound = OggController::getOggPlayer(m_deathSound);
      if(sound != NULL)
         Status stat = sound->play();
      
      shrink();
      shrink();
      shrink();
      if(m_hitpoints <= 2)
         markForRemoval();
      else 
         m_hitpoints -= 5;
   }
}






Level1Boss::Level1Boss(const Vector3& position, 
							const Vector3& rotation, 
							const CDH::Rectangle& polygon,
							GameEngine& engine) : 
	ScribbleEnemy(position, rotation, polygon, 100, true),
   m_engine(engine),
   m_jumpVelocity(75.0f)
{
	setDamping(0.9f);	
	setMass(2.0f);	
}
Level1Boss::~Level1Boss()
{}

Status 
Level1Boss::process()
{
  static CHUint firingTimer = 0;
   if( ++firingTimer >= 20 )
   {
      firingTimer = 0;
      fire();
   }   
   
   static CHUint jumpingTimer = 0;
   if( ++jumpingTimer >= 30 )
   {
      jumpingTimer = 0;
      jump();
   }

   // just move left for now.
	const float speed = 0.15f;
	m_oldPosition = position();
	static bool addIt = false;
   static float leftMost = 5.0f;
   static float rightMost = 10.0f;
   if( position().x <= leftMost )
      addIt = true;
   else if( position().x >= rightMost )
      addIt = false;
	
	setFacing( addIt ? FacingLeft : FacingRight );  // facing is backwards because of image
	setPosition( 
		Vector3( m_oldPosition.x + (addIt ? speed * 2.5f : -speed * 0.5f), 
					m_oldPosition.y, m_oldPosition.z));
	
	addForce(GRAVITY);
	integrate(0.25f);	

	if(sprite() != NULL)
		sprite()->animate();
   
   if(position().y < -12.0f)
   {
      setVelocity( Vector3(0.0f, 0.0f, 0.0f) );
      setPosition( position().x, -12.0f, 0.0f);
   }

	return StatusSuccess;
}
void
Level1Boss::jump()
{
   addForce(Vector3(0.0f, m_jumpVelocity, 0.0f));
}
void
Level1Boss::fire()
{
   Vector3 orientation( -1.0f, 0.0f, 0.0f);
   Vector3 projectilePosition( position().x ,
									    position().y + (m_polygon.height * 0.35f),
									    position().z);
   m_engine.createProjectile( Throwing, projectilePosition, orientation, team());
}
void 
Level1Boss::resolveCollisionWithProjectile( const Projectile& object )
{
   if(object.team() != m_team)
   {
      // shot by the hero
      --m_hitpoints;
      if(m_hitpoints == 0)
      {
         markForRemoval();
         OggPlayer* sound = OggController::getOggPlayer(m_deathSound);
         if(sound != NULL)
            Status stat = sound->play();
      }
      else
      {
         OggPlayer* sound = OggController::getOggPlayer(m_hitSound);
         if(sound != NULL)
            Status stat = sound->play();
      }
   }
   
}
void 
Level1Boss::resolveCollisionWithEnemy( const CharacterBase& object )
{
}