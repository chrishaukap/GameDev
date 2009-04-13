#ifdef _WINDOWS
#include <windows.h>
#endif

#include "peeps/peepBody.h"

#include <gl/gl.h>

PeepBody::PeepBody(const Vector3& position, 
                   const Vector3& orientation, 
                   const Vector3& rotation,
                   float scale) :
  AbstractPeep(position, orientation, rotation), 
  m_leftArmAngleY(sin(-PIOver4)), 
  m_rightArmAngleY(-sin(PIOver4)),
  m_leftLegAngleY(sin(-PIOver3)), 
  m_rightLegAngleY(-sin(PIOver3)),
  m_scale(scale) 
{
}

PeepBody::~PeepBody()
{
}

void PeepBody::renderDead() const
{
   render();
}

void PeepBody::render() const
{
   const float bodySize = m_scale;


   glPushMatrix();
   {
      GLfloat color[] = { 0.1f, 0.1f, 0.1f, 0.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

      glTranslatef(m_position.x, m_position.y - m_scale, m_position.z);
      glLineWidth(3);
      glScalef( m_scale, m_scale, m_scale );

      // BODY
      glPushMatrix();
      {        
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, -1.0f, 0.0f);
         glEnd();
      }
      glPopMatrix();

      // LEFT ARM
      glPushMatrix();
      {        
         glBegin( GL_LINES );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( -1 /*cos(PI)*/, m_leftArmAngleY, 0.0f);
         glEnd();
      }
      glPopMatrix();

      // RIGHT ARM
      glPushMatrix();
      {        
         glBegin( GL_LINES );
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 1 /*-cos(PI)*/, m_rightArmAngleY, 0.0f);
         glEnd();
      }
      glPopMatrix();

      // LEFT LEG
      glPushMatrix();
      {        
         glTranslatef(0.0f, -bodySize, 0.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( -1 /*cos(PI)*/, m_leftLegAngleY, 0.0f);
         glEnd();
      }
      glPopMatrix();

      // RIGHT LEG
      glPushMatrix();
      {             
         glTranslatef(0.0f, -bodySize, 0.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 1 /*-cos(PI)*/, m_rightLegAngleY, 0.0f);
         glEnd();
      }
      glPopMatrix();
      glLineWidth(1);
   }
   glPopMatrix();
}

// HAUKAP - 3 of the exact same functions.... 
BoundingCube PeepBody::getBoundingCube() const
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