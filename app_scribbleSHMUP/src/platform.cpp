#include "platform.h"

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

Platform::Platform(const Vector3& position, 
						 const Vector3& rotation, 
						 const CDH::Rectangle& polygon) : 
	ForegroundObject(position, rotation, polygon, ForegroundObject::Walkable)
{
		
}
Platform::~Platform()
{}

Status 
Platform::render() const
{
	const Frame &frame = sprite()->currentFrame();

	// HAUKAP - BLENDING DISABLED ON PLATFORMS FOR NOW
   glEnable(GL_TEXTURE_2D);		
	//glEnable(GL_BLEND);								
	glDisable(GL_DEPTH_TEST);	   
   {  
      float h = m_polygon.height;
      float w = m_polygon.width;
      float llx = position().x;
      float lly = position().y;
      float z = position().z;
   
      /*glBindTexture(GL_TEXTURE_2D, TextureManager::getTextureHandle( frame.mask ));	
      glBlendFunc(GL_DST_COLOR, GL_ZERO);	
      GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   glBegin(GL_QUADS);						
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
         glNormal3f( 0.0f, 0.0f, 1.0f );			
			glTexCoord2f(0.0f, 0.0f);   glVertex3f(llx,		lly,		z);	// Bottom Left
	      glTexCoord2f(w,    0.0f);   glVertex3f(llx + w,	lly,		z);	// Bottom Right
	      glTexCoord2f(w,    h);   glVertex3f(llx + w,	lly + h,	z);	// Top Right
	      glTexCoord2f(0.0f, h);   glVertex3f(llx,		lly + h,	z);	// Top Left
	   glEnd();	*/				

	   glBindTexture(GL_TEXTURE_2D, TextureManager::getTextureHandle( frame.img ));		
	   //glBlendFunc(GL_ONE, GL_ONE);				
      GLfloat color2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   glBegin(GL_QUADS);							
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color2);  
         glNormal3f( 0.0f, 0.0f, 1.0f );		
			glTexCoord2f(0.0f, 0.0f);   glVertex3f(llx,		lly,		z);	// Bottom Left
	      glTexCoord2f(w,    0.0f);   glVertex3f(llx + w,	lly,		z);	// Bottom Right
	      glTexCoord2f(w,    h);   glVertex3f(llx + w,	lly + h,	z);	// Top Right
	      glTexCoord2f(0.0f, h);   glVertex3f(llx,		lly + h,	z);	// Top Left
	   glEnd();	
   }
	glEnable(GL_DEPTH_TEST);						
	//glDisable(GL_BLEND);	   
   glDisable(GL_TEXTURE_2D);		

	return StatusSuccess;
}
