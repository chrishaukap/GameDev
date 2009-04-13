#ifdef _WINDOWS
#include <windows.h>
#endif

#include "blood/bloodEngine.h"
#include "blood/bloodSpout.h"
#include "blood/bloodDrop.h"

#include "openGLUtils.h"

#include <gl/gl.h>

BloodEngine::BloodEngine() : 
   m_droplets(), 
   m_spouts()
{}
BloodEngine::~BloodEngine() 
{   
   while( !m_droplets.empty() )   
   {
      BloodDrop* drop = m_droplets.back();
      delete drop;
      m_droplets.pop_back();
   }
   while( !m_spouts.empty() )
   {
      BloodSpout* spout = m_spouts.back();
      delete spout;
      m_spouts.pop_back();
   }
}

void BloodEngine::addDroplet(const Vector3& position,
                             const Vector3& orientation,
                             float speed,
                             float size,
                             float duration)
{
   m_droplets.push_back( new BloodDrop(position,orientation,speed,size, duration) );
}
BloodSpout* 
BloodEngine::addSpout(const Vector3& position,
                      const Vector3& orientation,
                      const Vector3& rotation,
                      BloodSpoutDensity density,
                      float speed,
                      float size,
                      float duration)
{ 
   BloodSpout* spout = 
      new BloodSpout(position, 
                     orientation, 
                     rotation, 
                     density, 
                     speed, 
                     size, 
                     duration );
   m_spouts.push_back(spout);
   return spout;
}

void BloodEngine::render() const
{
   CHUint sz = (CHUint)m_droplets.size();
   for( CHUint i=0; i<sz; ++i )
   {      
      glPushMatrix();
      {         
         const Vector3& p = m_droplets[i]->position;
         
         GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.0f };
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  

         glTranslatef( p.x, p.y, p.z);
         glScalef( m_droplets[i]->size, m_droplets[i]->size, m_droplets[i]->size );
         OpenGLUtils::glDrawFilledCircle(20, myglColor(1.0f, 0.0f, 0.0f) );
      }
      glPopMatrix();
   }   
}
void BloodEngine::process()
{
   CHUint sz = (CHUint)m_spouts.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if( m_spouts[i]->stopped() )
      {
         m_spouts.erase( m_spouts.begin() + i );
         sz = (CHUint)m_spouts.size();
         --i;
         continue;
      }
      m_spouts[i]->process();
   }



   // HAUKAP - TESTING...

   /*Vector3 orientation(0.0f, -1.0f, 0.0f);

   CHUint sz = 0;
   while( (sz = (CHUint)m_droplets.size()) <= 100 )
      addDroplet( Vector3(-15.0f, 25.0f, -15.0f),
                  utils3D::randomizeUnitVec(orientation, PIOver4),
                  (float)(rand() % 4) + 1,
                  0.2f,
                  5);

   for( CHUint i=0; i<sz; ++i )
   {
      m_droplets[i]->update();
      if(m_droplets[i]->expired() ||
         m_droplets[i]->position.y <= 0.0f)
      {
         m_droplets.erase( m_droplets.begin() + i );
         sz = (CHUint)m_droplets.size();
         --i;
      }
   }   */
}


CHUint BloodEngine::numSpouts() const
{
   return (CHUint)m_spouts.size();
}
CHUint BloodEngine::numDroplets() const
{
   CHUint total = 0;
   CHUint sz = (CHUint)m_spouts.size();
   for( CHUint i=0; i<sz; ++i )
      total += m_spouts[i]->numDroplets();

   return total;
}