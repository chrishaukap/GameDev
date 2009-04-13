#include "cdhBase.h"

#include "actor/characterBase.h"
#include "actor/projectile.h"
#include "actor/foregroundObject.h"
#include "math/vector3D.h"
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


CharacterBase::CharacterBase(const Vector3& position, 
									  const Vector3& rotation, 
									  const CDH::Rectangle& polygon,
									  CollidableType type,
									  CHUint team) : 
   GameObject(position, rotation, polygon, type),
	m_team(team),
   m_oldPosition(0.0f, 0.0f, 0.0f)
{
		
}
CharacterBase::~CharacterBase()
{}

void 
CharacterBase::resolveCollision( const Collidable& object )
{
	switch( object.collidableType() )
	{
	case Collidable::CollidableCharacter:
      {
         const CharacterBase* character = dynamic_cast<const CharacterBase*>(&object);
         if( team() != character->team() )
         {
            resolveCollisionWithEnemy(*character);
         }
      }
		break;
	case Collidable::CollidableProjectile: 
      {
         const Projectile* projectile = dynamic_cast<const Projectile*>(&object);
         if(projectile->team() != team())
            resolveCollisionWithProjectile(*projectile);
      }
		break;
	case Collidable::CollidableTerrain:
		{
			const ForegroundObject* terrain = dynamic_cast<const ForegroundObject*>(&object);
			resolveCollisionWithTerrain( *terrain );
		}
		break;
	default: 
		assert( !"invalid collidable type" );
	}
}


Status 
CharacterBase::render() const
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