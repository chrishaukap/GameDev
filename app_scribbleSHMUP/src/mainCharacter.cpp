#include "mainCharacter.h"
#include "actor/foregroundObject.h"

#include "cdhBase.h"
#include "input/keyInput.h"
#include "actor/projectile.h"
#include "gameEngine.h"
#include "sprite/sprite.h"

#include "oggController.h"
#include "oggPlayer.h"

#include <assert.h>

using namespace CDH;

const CHUint MainCharacter::LengthOfInvincibility = 20;
const CHUint MainCharacter::StartingHitPoints = 100;
const Vector3 MainCharacter::StartingLocation = Vector3(-10.0f, -12.0f, 0.0f);
CHUlong MainCharacter::m_deathSound = 0;
CHUlong MainCharacter::m_hitSound = 0;
CHUlong MainCharacter::m_jumpSound = 0;
CHUlong MainCharacter::m_hurt1Sound = 0;
CHUlong MainCharacter::m_hurt2Sound = 0;
CHUlong MainCharacter::m_pickupSound = 0;

MainCharacter::MainCharacter(GameEngine& engine,
									const KeyInput& input, 
									const Vector3& position, 
									const Vector3& rotation, 
									const CDH::Rectangle& polygon) : 
   KeyCharacter(input, position, rotation, polygon),
	m_engine(engine),
	m_jumpVelocity(75.0f),
	m_moveSpeed(0.5f),
	m_isJumping(false),
	m_hitpoints(StartingHitPoints),
   m_numLives(3),
   m_invincible(false),
   m_invincibilityCounter(0)
{
	setDamping(0.9f);	
	setMass(2.0f);
}
MainCharacter::~MainCharacter()
{}

Status 
MainCharacter::process()
{  
   if(m_invincible && --m_invincibilityCounter == 0)
      m_invincible = false;

	m_oldPosition = position();
   if( m_input.keyPressed(Idx_LEFT) || m_input.keyPressed(Idx_RIGHT))
   {
      if(m_input.keyPressed(Idx_LEFT))
		{
			setFacing(FacingLeft);
         setPosition( m_oldPosition.x - m_moveSpeed, m_oldPosition.y, m_oldPosition.z );
		}
      if(m_input.keyPressed(Idx_RIGHT))
		{
			setFacing(FacingRight);
         setPosition( m_oldPosition.x + m_moveSpeed, m_oldPosition.y, m_oldPosition.z );
		}
      
      sprite()->animate();
   }

   if( m_input.keyPressed(Idx_D))
   {			
      if( !m_isJumping)
		{
			addForce(Vector3(0.0f, m_jumpVelocity, 0.0f));
			m_isJumping = true;

         OggPlayer* jumpSound = OggController::getOggPlayer(m_jumpSound);
         if(jumpSound)
            jumpSound->play();
		}
	}			

   static CHUint firingTimer = 0;
   if( m_input.keyPressed(Idx_F))
   {		
      if( ++firingTimer > 6 )
      {
         firingTimer = 0;
         Vector3 orientation( (facing() == FacingRight) ?  1.0f : -1.0f, 0.0f, 0.0f);
		   Vector3 projectilePosition( facing() == FacingRight ? 
                                        position().x + m_polygon.width : 
                                        position().x,
											    position().y + (m_polygon.height * 0.5f),
											    position().z);
		   Status stat = m_engine.createProjectile( Throwing, projectilePosition, orientation, team());
      }
	}
   else
      firingTimer = 6;

	// process physics
	addForce(GRAVITY);
	integrate(0.25f);	

	return StatusSuccess;
}

void 
MainCharacter::resolveCollisionWithTerrain( const ForegroundObject& object )
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
				m_isJumping = false;
			}
		}
	}
}

CHUlong
MainCharacter::hurtSound() const
{
   int sound = rand() % 3;
   OGG_HANDLE h=0;
   switch(sound)
   {
   case 0: h = m_hurt1Sound; break;
   case 1: h = m_hurt2Sound; break;
   case 2: h = m_hitSound; break;
   default: assert( !"??" );
   }
   return h;
}
void
MainCharacter::gainLives(CHUint lives)
{
   m_numLives += lives;
}

void
MainCharacter::respawn()
{
   assert(m_numLives > 0);
   m_hitpoints = StartingHitPoints;
   setPosition(StartingLocation);
   setVelocity(Vector3(0.0f, 0.0f, 0.0f));
   clearAccumulator();
   unmark();
}

CHUint
MainCharacter::numLives() const
{
   return m_numLives;
}
bool
MainCharacter::canRespawn() const 
{
   return (m_numLives > 0);
}
void
MainCharacter::decrementLife()
{
   assert(m_numLives > 0);
   if(m_numLives > 0)
      --m_numLives;
}


void 
MainCharacter::resolveCollisionWithProjectile( const Projectile& object )
{
   if(object.team() != m_team)
   {      
      if(!m_invincible)
      {		
         m_hitpoints -= 5; // HAUKAP - go invincible for a time?  jump back?  play different animation?  play sound ? etc.
         if(m_hitpoints > 0)
         {
            m_invincible = true;
            m_invincibilityCounter = LengthOfInvincibility;    
            OggController::getOggPlayer( hurtSound() )->play();       
            
            // bounce back
            Vector3 newPos(position());
            newPos.x -= 2.0f;
            setPosition(newPos);
         }
         else
         {
            OggController::getOggPlayer( m_deathSound )->play();

            markForRemoval();
         }
      }
   }   
}

void
MainCharacter::resolveCollisionWithEnemy( const CharacterBase& object )
{
	if(team() != object.team() )
	{
      if( oldPosition().y > object.position().y + object.polygon().height)
      {
         // jump off the enemy
         clearAccumulator();
         addForce(Vector3(0.0f, 1.5f * m_jumpVelocity, 0.0f));      
      }
      else
      {
         if(!m_invincible)
         {		
            m_hitpoints -= 5; // HAUKAP - go invincible for a time?  jump back?  play different animation?  play sound ? etc.
            if(m_hitpoints > 0)
            {
               m_invincible = true;
               m_invincibilityCounter = LengthOfInvincibility;    
               OggController::getOggPlayer( hurtSound() )->play();    
            }
            else
            {
               OggController::getOggPlayer( m_deathSound )->play();

               markForRemoval();
            }

         }
      }
   }
}