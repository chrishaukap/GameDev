#ifdef _WINDOWS
#include <windows.h>
#endif

#include "cdhBase.h"
using namespace CDH;

#include <gl/gl.h>

#include "blood/bloodSpout.h"
#include "blood/bloodDrop.h"
#include "openGLUtils.h"

BloodSpout::~BloodSpout()
{
   while( !m_droplets.empty() )
   {
      BloodDrop* spout = m_droplets.back();
      delete spout;
      m_droplets.pop_back();
   }   
}

void
BloodSpout::render() const
{
   CHUint sz = (CHUint)m_droplets.size();
   for( CHUint i=0; i<sz; ++i )
   {      
      glPushMatrix();
      {         
         GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.0f };
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

         const Vector3& p = m_droplets[i]->position;
         glTranslatef( p.x, p.y, p.z);
         glScalef( m_droplets[i]->size, m_droplets[i]->size, m_droplets[i]->size );
         OpenGLUtils::glDrawFilledCircle(20, myglColor(1.0f, 0.0f, 0.0f) );
      }
      glPopMatrix();
   }   
}

void 
BloodSpout::setPosition(const Vector3& position)
{
   Vector3 diff = position - m_position;
   m_position = position;

   CHUint sz = (CHUint)m_droplets.size();
   for( CHUint i=0; i<sz; ++i )
      m_droplets[i]->position += diff;
}

void
BloodSpout::process()
{
   CHUint maxDroplets = 0;
   switch(m_density)
   {
   case Sparse: maxDroplets = 30; break;
   case Normal: maxDroplets = 60; break;
   case Dense: maxDroplets = 100; break;
   }

   CHUint sz = 0;
   while( (sz = (CHUint)m_droplets.size()) <= maxDroplets )
      m_droplets.push_back(
         new BloodDrop( m_position, 
                        utils3D::randomizeUnitVec(m_orientation, PIOver6), 
                        m_dropletSpeed * (float)((double)rand() / ((double)(RAND_MAX)+(double)(1)) ),
                        m_dropletSize,
                        8 /* HAUKAP - DEFINE THIS ELSEWHERE */ ) );

   for( CHUint i=0; i<sz; ++i )
   {
      m_droplets[i]->update();
      if(m_droplets[i]->expired())
      {
         m_droplets.erase( m_droplets.begin() + i );
         sz = (CHUint)m_droplets.size();
         --i;
      }
   }   
}

CHUint BloodSpout::numDroplets() const
{
   return (CHUint)m_droplets.size();
}