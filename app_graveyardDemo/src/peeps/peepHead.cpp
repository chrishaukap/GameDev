#ifdef _WINDOWS
#include <windows.h>
#endif

#include "cdhBase.h"
using namespace CDH;

#include "peeps/peepHead.h"
#include "openGLUtils.h"

#include <gl/gl.h>

// position is the center of where the head will be. 
PeepHead::PeepHead(const Vector3& position, 
                   const Vector3& orientation, 
                   const Vector3& rotation,
                   float scale) : 
   AbstractPeep(position,orientation, rotation),
   m_scale(scale), 
   m_mouthScale(0.35f), 
   m_eyeScale(0.1f) 
{}
PeepHead::~PeepHead(){}
      
void PeepHead::renderDead() const
{
   glPushMatrix();
   {
      glTranslatef( m_position.x, m_position.y, m_position.z );
      glScalef( m_scale, m_scale, m_scale );
      renderFace();
      renderMouth();
      renderDeadEyes();
   }
   glPopMatrix();

}
void PeepHead::render(void) const
{
   // head
   glPushMatrix();
   {
      glTranslatef( m_position.x, m_position.y, m_position.z );
      glScalef( m_scale, m_scale, m_scale );
      renderFace();
      renderMouth();
      renderEyes();
   }
   glPopMatrix();
}

void PeepHead::renderMouth() const
{
   // mouth
   glPushMatrix();
   {           
      GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( 0.0f, -0.35f, 0.1f );
      glScalef( m_mouthScale, m_mouthScale, m_mouthScale );
      glNormal3f( 0.0f, 0.0f, 1.0f );
      OpenGLUtils::glDrawFilledCircle(20, myglColor(0.0f, 0.0f, 0.0f) );
   }
   glPopMatrix();
}
void PeepHead::renderFace() const
{
   glPushMatrix();
   {
      GLfloat color[] = { 0.25f, 0.25f, 0.25f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( 0.0f, 0.0f, 0.0f );
      glScalef( 0.5f, 1.0f, 1.0f );
      glNormal3f( 0.0f, 0.0f, 1.0f );
      OpenGLUtils::glDrawFilledCircle(20, myglColor(0.8f, 0.8f, 0.8f) );
   }
   glPopMatrix();
}
void PeepHead::renderEyes() const
{
   // left eye
   glPushMatrix();
   {      
      GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( -0.25f, 0.35f, 0.1f );
      glScalef( m_eyeScale, m_eyeScale, m_eyeScale );
      glNormal3f( 0.0f, 0.0f, 1.0f );
      OpenGLUtils::glDrawFilledCircle(20, myglColor(0.0f, 0.0f, 0.0f) );
   }
   glPopMatrix();

   // right eye
   glPushMatrix();
   {
      GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( 0.25f, 0.35f, 0.1f );
      glScalef( m_eyeScale, m_eyeScale, m_eyeScale );
      glNormal3f( 0.0f, 0.0f, 1.0f );
      OpenGLUtils::glDrawFilledCircle(20, myglColor(0.0f, 0.0f, 0.0f) );
   }
   glPopMatrix();
}
void PeepHead::renderDeadEyes() const
{
   // left eye
   glPushMatrix();
   {
      GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( -0.25f, 0.35f, 0.1f );
      glScalef( m_eyeScale, m_eyeScale, m_eyeScale );
      glLineWidth(2.0f);
      glBegin( GL_LINES );
      {
         glVertex3f( -1.0f, 1.0f, 0.0f );
         glVertex3f( 1.0f, -1.0f, 0.0f );
         glVertex3f( -1.0f, -1.0f, 0.0f );
         glVertex3f( 1.0f, 1.0f, 0.0f );
      }
      glEnd();
      glLineWidth(1.0f);
   }
   glPopMatrix();

   // right eye
   glPushMatrix();
   {
      GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef( 0.25f, 0.35f, 0.1f );
      glScalef( m_eyeScale, m_eyeScale, m_eyeScale );
      glBegin( GL_LINES );
      {
         glVertex3f( -1.0f, 1.0f, 0.0f );
         glVertex3f( 1.0f, -1.0f, 0.0f );
         glVertex3f( -1.0f, -1.0f, 0.0f );
         glVertex3f( 1.0f, 1.0f, 0.0f );
      }
      glEnd();
   }
   glPopMatrix();
}

BoundingCube PeepHead::getBoundingCube() const
{
   const Vector3& pos = m_position;
   return BoundingCube(
      pos.x - (PEEP_BOUNDING_HALFWIDTH * m_scale), // HAUKAP - shouldn't need define here... should be part of class
      pos.x + (PEEP_BOUNDING_HALFWIDTH * m_scale),
      pos.y - (PEEP_BOUNDING_HALFWIDTH * m_scale),
      pos.y + (PEEP_BOUNDING_HALFWIDTH * m_scale),
      pos.z - (PEEP_BOUNDING_HALFWIDTH * m_scale),
      pos.z + (PEEP_BOUNDING_HALFWIDTH * m_scale)
      );
}