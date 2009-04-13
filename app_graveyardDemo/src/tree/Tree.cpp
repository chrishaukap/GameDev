#ifdef _WINDOWS
#include <windows.h>
#endif

#include "tree/tree.h"
#include "tree/limb.h"
#include "assert.h"
#include <vector>

#include <gl/gl.h>

extern GLuint g_texture[];

Tree::Tree(const Vector3& position) : 
   m_limbs(),
   m_position(position)
{   
   Vector3 limbOrientation(1.0f, 0.0f, 0.0f);
   Vector3 location(m_position.x, m_position.y + 10.0f, m_position.z);

   for(CHUint i=0; i<MAX_LIMBS; ++i)
      m_limbs.push_back( new Limb(location, limbOrientation) );
}

Tree::~Tree()
{
   CHUint sz = (CHUint)m_limbs.size();
   for(CHUint i=0; i<sz; ++i)
      delete m_limbs[i];
   m_limbs.clear();
}   

bool Tree::canPursue() const
{
   CHUint sz = (CHUint)m_limbs.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if( m_limbs[i]->isFree() )
         return true;
   }
   return false;
}
void Tree::pursue( MoveableObject& moveableObject)
{
   CHUint sz = (CHUint)m_limbs.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if( m_limbs[i]->isFree() )
      {
         m_limbs[i]->assignTarget(moveableObject);
         break;
      }
   }
}

void Tree::process(bool &targetAquired)
{
   targetAquired = false;

   CHUint sz = (CHUint)m_limbs.size();
   for( CHUint i=0; i<sz; ++i )
   {
      m_limbs[i]->growBranches();
      if(m_limbs[i]->isPursuing())
      {
         m_limbs[i]->move();
         m_limbs[i]->grow();
         if(m_limbs[i]->targetAquired())
         {
            targetAquired = true;
            m_limbs[i]->thrash();
         }
      }
      else if(m_limbs[i]->isThrashing())
      {
         m_limbs[i]->thrash();
      }
   }   
}

void Tree::render() const
{   	
   glEnable(GL_TEXTURE_2D);		
	glEnable(GL_BLEND);								
	glDisable(GL_DEPTH_TEST);	 

   static CHUint treeAnimCounter = 0;
   static CHUint treeAnimIdx = 0;
   if(++treeAnimCounter >= 5)
   {
      treeAnimCounter = 0;
      if( ++treeAnimIdx > 1)
         treeAnimIdx = 0;
   }

   glPushMatrix();
   {
      float txtHeightMult = 1.0f;
      float txtWidthMult = 1.0f; 
      float aspectRatio = 1.0f;
      float height = 15.0f;
      float width = 15.0f;
      float llx = position().x - (width/2);
      float lly = 0.0f;
      float z = -10.0f;
      
      // HAUKAP - magic numbers.... should get these from a texture manager
      GLuint maskIdx = 7;
      GLuint textureIdx = treeAnimIdx == 1 ? 5 : 14; 
      
	   glBindTexture(GL_TEXTURE_2D, g_texture[maskIdx]);	
      glBlendFunc(GL_DST_COLOR,GL_ZERO);	
      GLfloat color[] = { 1.0f, 1.0f, 1.0f, 0.8f };
	   glBegin(GL_QUADS);						
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
         glNormal3f( 0.0f, 0.0f, 1.0f );	
	      glTexCoord2f( 0.0f,           0.0f);            glVertex3f(llx,          lly,           z);	// Bottom Left
	      glTexCoord2f( txtWidthMult,   0.0f);            glVertex3f(llx + width,  lly,           z);	// Bottom Right
	      glTexCoord2f( txtWidthMult,   txtHeightMult);   glVertex3f(llx + width,  lly + height,  z);	// Top Right
	      glTexCoord2f( 0.0f,           txtHeightMult);   glVertex3f(llx,          lly + height,  z);	// Top Left
	   glEnd();

	   glBindTexture(GL_TEXTURE_2D, g_texture[textureIdx]);		
	   glBlendFunc(GL_ONE, GL_ONE);				
      GLfloat color2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   glBegin(GL_QUADS);							
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color2);  
         glNormal3f( 0.0f, 0.0f, 1.0f );
		   glTexCoord2f( 0.0f,           0.0f);            glVertex3f(llx,          lly,           z);	// Bottom Left
		   glTexCoord2f( txtWidthMult,   0.0f);            glVertex3f(llx + width,  lly,           z);	// Bottom Right
		   glTexCoord2f( txtWidthMult,   txtHeightMult);   glVertex3f(llx + width,  lly + height,  z);	// Top Right
		   glTexCoord2f( 0.0f,           txtHeightMult);   glVertex3f(llx,          lly + height,  z);	// Top Left
	   glEnd();	

   }
   glPopMatrix();

	glEnable(GL_DEPTH_TEST);						
	glDisable(GL_BLEND);	   
   glDisable(GL_TEXTURE_2D);	

   CHUint sz = (CHUint)m_limbs.size();
   for( CHUint i=0; i<sz; ++i )
   {
      m_limbs[i]->render();
   }    
}