#include "sprite/background.h"
#include "resource/resourceLibrary.h"
#include "resource/textureManager.h"
#include "resource/imageBase.h"

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <gl/gl.h>


using namespace CDH;

Background::Background() : 
	m_background(0),
	m_x(0),
	m_y(0),
   m_rate(0.0f)
{}
Background::~Background()
{}

void Background::setBackground(RES_HANDLE bg, float x, float y, float rate)
{
	m_background = bg;
	m_x = x;
	m_y = y;
   m_rate = rate;
}

Status Background::render() const
{
	Status stat = StatusFileNotFound;

	const ImageBase* bg = ResourceLibrary::getImage(m_background);
	if(bg != NULL)
	{		
      glEnable(GL_TEXTURE_2D);						
	   glDisable(GL_DEPTH_TEST);	   
      {  
         float aspectRatio = 2.0f;
         float height = 30.0f;
         float width = height * aspectRatio;
         float llx = m_x;
         float lly = m_y;
         float z = 0.0f;
      
         static float scroll = 0.0f;
         scroll += m_rate;

         glBindTexture(GL_TEXTURE_2D, TextureManager::getTextureHandle( m_background ));		
	      GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	      glBegin(GL_QUADS);							
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
            glNormal3f( 0.0f, 0.0f, 1.0f );
		      glTexCoord2f( 0.0f + scroll,   0.0f);   glVertex3f(llx,          lly,           z);	// Bottom Left
		      glTexCoord2f( 1.0f + scroll,   0.0f);   glVertex3f(llx + width,  lly,           z);	// Bottom Right
		      glTexCoord2f( 1.0f + scroll,   1.0f);   glVertex3f(llx + width,  lly + height,  z);	// Top Right
		      glTexCoord2f( 0.0f + scroll,   1.0f);   glVertex3f(llx,          lly + height,  z);	// Top Left
	      glEnd();	
      }
	   glEnable(GL_DEPTH_TEST);		
      glDisable(GL_TEXTURE_2D);	

      stat = StatusSuccess;
	}

	return stat;
}
   