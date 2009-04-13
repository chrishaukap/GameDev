#ifdef _WINDOWS
#include <windows.h>
#endif

#include "fence.h"
#include <math.h>
#include <gl/gl.h>

const int FenceSegment::NumSpikes = 5;   

FenceSegment::FenceSegment(const Vector3& point, float angle, float scale) : 
   m_point(point), m_fenceAngle(angle), m_scale(scale)
{
   // populate spikes     
   const float lowAngle = getAngle() + PIOver4;
   const float highAngle = getAngle() - PIOver4;
      
   m_spikeAngles = new float[NumSpikes];
   for( int i=0; i<NumSpikes; ++i )
   {
      double randPercent = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
      m_spikeAngles[i] = RAD2DEG( lowAngle + ((float)randPercent * (highAngle - lowAngle)) );               
   }  
}
   
FenceSegment::~FenceSegment()
{
   delete m_spikeAngles;
}
float FenceSegment::getX() const 
{
   return m_point.x;
}
float FenceSegment::getY() const 
{
   return m_point.y;
}
float FenceSegment::getZ() const 
{
   return m_point.z;
}
void FenceSegment::setX(float x) 
{
   m_point.x = x;
}
void FenceSegment::setY(float y) 
{
   m_point.y = y;
}
void FenceSegment::setZ(float z) 
{
   m_point.z = z;
}
float FenceSegment::getAngle() const 
{
   return m_fenceAngle;
}
void FenceSegment::setAngle(float angle) 
{
   m_fenceAngle = angle;
}
float FenceSegment::getScale() const 
{
   return m_scale;
}
void FenceSegment::setScale(float scale) 
{
   m_scale = scale;
}
void FenceSegment::render() const
{
   // HAUKAP - need spike::render to reduce code
   glPushMatrix();
   {
      glTranslatef( m_point.x, m_point.y, m_point.z);
      glRotatef( m_fenceAngle, 0.0f, 0.0f, 1.0f);
      glLineWidth(1);
      //glColor3f(0.0f, 0.0f, 0.0f);
      glScalef( m_scale, m_scale, m_scale );

      // first spike 
      glPushMatrix();
      {
         glRotatef( m_spikeAngles[0], 0.0f, 0.0f, 1.0f);         
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, 1.0, 0.0f);
         glEnd();  
      }
      glPopMatrix();

      // second spike 
      glPushMatrix();
      {
         glRotatef( m_spikeAngles[1], 0.0f, 0.0f, 1.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, 1.0, 0.0f);
         glEnd();  
      }
      glPopMatrix();
      
      // third spike 
      glPushMatrix();
      {
         glRotatef( m_spikeAngles[2], 0.0f, 0.0f, 1.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, 1.0, 0.0f);
         glEnd();  
      }
      glPopMatrix();

      // fourth spike 
      glPushMatrix();
      {
         glRotatef( m_spikeAngles[3], 0.0f, 0.0f, 1.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, 1.0, 0.0f);
         glEnd();  
      }
      glPopMatrix();

      // fifth spike 
      glPushMatrix();
      {
         glRotatef( m_spikeAngles[4], 0.0f, 0.0f, 1.0f);
         glBegin( GL_LINES );
            glVertex3f( 0, 0, 0.0f );
            glVertex3f( 0, 1.0, 0.0f);
         glEnd();  
      }
      glPopMatrix();
   }
   glPopMatrix();

}