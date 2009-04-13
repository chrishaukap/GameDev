#ifdef _WINDOWS
#include <windows.h>
#endif

#include "peeps/peep.h"
#include "peeps/peepHead.h"
#include "peeps/peepBody.h"

#include "graveyard.h"
#include <gl/gl.h>
#include <assert.h>

extern GLuint g_texture[];

enum PeepType2
{
   Peep1,
   Peep2,
   NumPeepTypes
};

Peep::Peep(const Vector3& position, 
           const Vector3& orientation, 
           const Vector3& rotation, 
           float scale) : 
   AbstractPeep(position, orientation, rotation),
   m_head(new PeepHead(position, orientation, rotation, scale)),
   m_body(new PeepBody(position, orientation, rotation, scale)),
   m_animCounter(0),
   m_animFrameIdx(0),
   m_ownsParts(true)
{
   m_type = (PeepType2) (rand() % NumPeepTypes);
}
   
Peep::~Peep()
{
   if(m_ownsParts)
   {
      delete m_head;
      delete m_body;
   }
}

float Peep::scale() const 
{
   assert(m_head != NULL);
   return m_head->scale();
}
void Peep::setScale(float scale) 
{
   assert(m_head != NULL);
   m_head->setScale(scale);
}
void Peep::flipZOrientation()
{
   Vector3 newVec = orientation();
   newVec.z = -newVec.z;
   setOrientation( newVec );
}
void Peep::flipXOrientation()
{
   Vector3 newVec = orientation();
   newVec.x = -newVec.x;
   setOrientation( newVec );
}
void Peep::move()
{
   Vector3 oldPos = position();
   setPosition(utils3D::translatePointAlongVector( position(), orientation(), PEEP_SPEED ));
   BoundingCube cube = getBoundingCube();

   float newX = position().x;
   float newZ = position().y;
   bool horizCollision = false, vertCollision = false;
   if(cube.uX > GraveyardBounds::rightWallX)
   {
      horizCollision = true;
      newX = GraveyardBounds::rightWallX - PEEP_BOUNDING_HALFWIDTH;
   }
   else if( cube.lX < GraveyardBounds::leftWallX )
   {
      horizCollision = true;
      newX =  GraveyardBounds::leftWallX + PEEP_BOUNDING_HALFWIDTH;
   }

   if(cube.uZ > GraveyardBounds::frontWallZ)
   {
      vertCollision = true;
      newZ = GraveyardBounds::frontWallZ + PEEP_BOUNDING_HALFWIDTH;
   }
   else if( cube.lZ < GraveyardBounds::backWallZ )
   {
      vertCollision = true;
      newZ = GraveyardBounds::backWallZ - PEEP_BOUNDING_HALFWIDTH;
   }

   if( horizCollision || vertCollision )
   {
      setPosition(oldPos);

      if(horizCollision)
         flipXOrientation();
      if(vertCollision)
         flipZOrientation();
   }
}

void Peep::Split(PeepHead*& head, PeepBody*& body)
{
   m_ownsParts = false;
   head = m_head;
   body = m_body;
}

void Peep::RenderBoundingCube() const
{
   BoundingCube cube = getBoundingCube();

   glPushMatrix();
   {
      //glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_LINE_LOOP);
      {
         glVertex3f( cube.lX, cube.uY, cube.lZ ); 
         glVertex3f( cube.uX, cube.uY, cube.lZ ); 
         glVertex3f( cube.uX, cube.uY, cube.uZ ); 
         glVertex3f( cube.lX, cube.uY, cube.uZ ); 
      }
      glEnd();   
      glBegin(GL_LINE_LOOP);
      {
         glVertex3f( cube.lX, cube.lY, cube.lZ ); 
         glVertex3f( cube.uX, cube.lY, cube.lZ ); 
         glVertex3f( cube.uX, cube.lY, cube.uZ ); 
         glVertex3f( cube.lX, cube.lY, cube.uZ ); 
      }
      glEnd();   
      glBegin(GL_LINES);
      {
         glVertex3f( cube.lX, cube.uY, cube.lZ ); 
         glVertex3f( cube.lX, cube.lY, cube.lZ ); 
      }
      glEnd();   
      glBegin(GL_LINES);
      {
         glVertex3f( cube.uX, cube.uY, cube.lZ ); 
         glVertex3f( cube.uX, cube.lY, cube.lZ ); 
      }
      glEnd();   
      glBegin(GL_LINES);
      {
         glVertex3f( cube.uX, cube.uY, cube.uZ ); 
         glVertex3f( cube.uX, cube.lY, cube.uZ ); 
      }
      glEnd();   
      glBegin(GL_LINES);
      {
         glVertex3f( cube.lX, cube.uY, cube.uZ ); 
         glVertex3f( cube.lX, cube.lY, cube.uZ ); 
      }
      glEnd();   
      //glColor3f(0.0f, 0.0f, 0.0f);
   }
   glPopMatrix();
}

void Peep::renderDead() const
{
   m_head->renderDead();
   m_body->renderDead();
}

BoundingCube Peep::getBoundingCube() const
{
   // bounding cube only includes the head currently.
   const Vector3& pos = m_position;
   return BoundingCube(
      pos.x - (PEEP_BOUNDING_HALFWIDTH), // HAUKAP - shouldn't need define here... should be part of class
      pos.x + (PEEP_BOUNDING_HALFWIDTH),
      pos.y - (PEEP_BOUNDING_HALFWIDTH),
      pos.y + (PEEP_BOUNDING_HALFWIDTH),
      pos.z - (PEEP_BOUNDING_HALFWIDTH),
      pos.z + (PEEP_BOUNDING_HALFWIDTH)
      );
}


void 
Peep::setOrientation(const Vector3& orientation) 
{ 
   m_orientation = orientation;
   m_head->setOrientation(m_orientation);
   m_body->setOrientation(m_orientation);
}
const Vector3& 
Peep::orientation() const 
{ 
   return m_head->orientation(); 
}
void 
Peep::setPosition(const Vector3& position) 
{ 
   Particle::setPosition(position); 
   m_head->setPosition(position);
   m_body->setPosition(position);
}
const Vector3& 
Peep::position() const 
{ 
   return m_head->position(); 
}

void Peep::animate()
{
   if( ++m_animCounter >= 2 )
   {
      m_animCounter = 0;
      switch(m_type)
      {
      case Peep1: 
         if(++m_animFrameIdx >= 2) //HAUKAP - MAGIC NUMBER
            m_animFrameIdx = 0;
         break;
      case Peep2:
         if(++m_animFrameIdx >= 3) //HAUKAP - MAGIC NUMBER
            m_animFrameIdx = 0;
         break;
      default: assert(!"invalid peep type");break;
      }
      
   }
}

void Peep::getAnimFrameIdx(CHUint &maskIdx, CHUint &txtIdx) const
{
   // HAUKAP - peep types should derive from a generic peep type
   switch(m_type)
   {
   case Peep1:
      switch(m_animFrameIdx)
      {
         // HAUKAP - magic numbers.... should get these from a texture manager
      case 0: 
         maskIdx = 0;
         txtIdx = 1;
         break;
      case 1: 
         maskIdx = 8;
         txtIdx = 9;
         break;
      default: 
         assert( !"invalid animation frame index");
      }
      break;
   case Peep2:
      switch(m_animFrameIdx)
      {
         // HAUKAP - magic numbers.... should get these from a texture manager
      case 0: 
         maskIdx = 10;
         txtIdx = 11;
         break;
      case 1: 
         maskIdx = 12;
         txtIdx = 13;
         break;
      case 2: 
         maskIdx = 2;
         txtIdx = 3;
         break;
      default: 
         assert( !"invalid animation frame index");
      }
      break;
   default:
      assert( !"Invalid Peep type");
   }
}

void Peep::render() const
{  
   glEnable(GL_TEXTURE_2D);		
	glEnable(GL_BLEND);								
	glDisable(GL_DEPTH_TEST);	   
   {      
      float txtHeightMult = 1.0f;
      float txtWidthMult = 1.0f; 
      float aspectRatio = 1.0f;
      float height = 0.0f;
      float width = 0.0f;
      float llx = position().x;
      float lly = position().y;
      float z = position().z;
      GLuint maskIdx = 0;
      GLuint textureIdx = 0;

      // HAUKAP - this should go into initializer
      switch(m_type)
      {
      case Peep1: 
         height = 3.0f;
         aspectRatio = txtWidthMult = 0.41f;
         width = height * aspectRatio;
         getAnimFrameIdx(maskIdx, textureIdx);
         break;
      case Peep2: 
         width = 2.5f;
         txtHeightMult = 0.7f;
         txtWidthMult = 0.94f;
         aspectRatio = 358.0f / 483.0f;
         height = width * aspectRatio;
         getAnimFrameIdx(maskIdx, textureIdx);
         break;
      default: 
         assert(0);
         break;
      };
      assert( height != 0.0f && width != 0.0f );

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
	glEnable(GL_DEPTH_TEST);						
	glDisable(GL_BLEND);	   
   glDisable(GL_TEXTURE_2D);		

#ifdef DRAW_PEEPS_BBOX
      RenderBoundingCube();
#endif
}
