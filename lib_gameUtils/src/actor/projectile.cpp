#include "cdhBase.h"
#include "actor/projectile.h"
#include "actor/characterBase.h"
#include <assert.h>

// for rendering capabilities
#include "resource/imageBase.h"
#include "resource/resourceLibrary.h"
#include "resource/textureManager.h"
#include "sprite/sprite.h"
#include "sprite/animation.h"
#ifdef _WINDOWS
#include <windows.h>
#endif
#include <gl/gl.h>

using namespace CDH;

Projectile::Projectile(const Vector3& position, 
							  const Vector3& rotation, 
							  const Vector3& orientation,
							  float velocity,
							  float mass,
							  const CDH::Rectangle& polygon,
							  CHUint team) : 
	GameObject(position, rotation, polygon, CollidableProjectile),
	m_team(team),
   m_lifetime(30)
{
	Vector3 velocityVector(orientation);
	velocityVector *= velocity;
	setVelocity( velocityVector );

	setMass(mass);
	setDamping(0.9f);
	clearAccumulator();
}
Projectile::~Projectile()
{}

Status 
Projectile::process()
{
   if(-- m_lifetime == 0)
      markForRemoval();

	const float duration = 0.25f; // HAUKAP - this should be stored away somewhere to control the rate of gameplay
	integrate(duration);

	assert(m_sprite!= NULL);
	sprite()->animate();

	return StatusSuccess;
}

Status 
Projectile::render() const
{	     
	const Frame &frame = sprite()->currentFrame();

   glEnable(GL_TEXTURE_2D);		
	glEnable(GL_BLEND);								
	glDisable(GL_DEPTH_TEST);	   
   {  
      float h = m_polygon.height;
      float w = m_polygon.width;
      float llx = position().x;
      float lly = position().y;
      float z = position().z;
   
      glBindTexture(GL_TEXTURE_2D, TextureManager::getTextureHandle( frame.mask ));	
      glBlendFunc(GL_DST_COLOR, GL_ZERO);	
      GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   glBegin(GL_QUADS);						
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
         glNormal3f( 0.0f, 0.0f, 1.0f );			
			glTexCoord2f( (m_facing == FacingRight) ? 0.0f : 1.0f,   0.0f);   glVertex3f(llx,		lly,		z);	// Bottom Left
	      glTexCoord2f( (m_facing == FacingRight) ? 1.0f : 0.0f,   0.0f);   glVertex3f(llx + w,	lly,		z);	// Bottom Right
	      glTexCoord2f( (m_facing == FacingRight) ? 1.0f : 0.0f,   1.0f);   glVertex3f(llx + w,	lly + h,	z);	// Top Right
	      glTexCoord2f( (m_facing == FacingRight) ? 0.0f : 1.0f,   1.0f);   glVertex3f(llx,		lly + h,	z);	// Top Left
	   glEnd();					

	   glBindTexture(GL_TEXTURE_2D, TextureManager::getTextureHandle( frame.img ));		
	   glBlendFunc(GL_ONE, GL_ONE);				
      GLfloat color2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   glBegin(GL_QUADS);							
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color2);  
         glNormal3f( 0.0f, 0.0f, 1.0f );	
			glTexCoord2f( (m_facing == FacingRight) ? 0.0f : 1.0f,   0.0f);   glVertex3f(llx,		lly,		z);	// Bottom Left
	      glTexCoord2f( (m_facing == FacingRight) ? 1.0f : 0.0f,   0.0f);   glVertex3f(llx + w,	lly,		z);	// Bottom Right
	      glTexCoord2f( (m_facing == FacingRight) ? 1.0f : 0.0f,   1.0f);   glVertex3f(llx + w,	lly + h,	z);	// Top Right
	      glTexCoord2f( (m_facing == FacingRight) ? 0.0f : 1.0f,   1.0f);   glVertex3f(llx,		lly + h,	z);	// Top Left
	    glEnd();	
   }
	glEnable(GL_DEPTH_TEST);						
	glDisable(GL_BLEND);	   
   glDisable(GL_TEXTURE_2D);		

   return StatusSuccess;
}

void Projectile::resolveCollision( const Collidable& object )
{
	switch( object.collidableType() )
	{
	case Collidable::CollidableCharacter:
		{
			const CharacterBase* character = dynamic_cast<const CharacterBase*>(&object);
			if(character->team() != m_team)
				markForRemoval();
		}
		break;
	case Collidable::CollidableProjectile: 
		break;
	case Collidable::CollidableTerrain:
		break;
	default: 
		assert( !"invalid collidable type" );
	}
}