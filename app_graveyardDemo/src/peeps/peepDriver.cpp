#include "cdhBase.h"
using namespace CDH;

#include "peeps/peepDriver.h"
#include "peeps/peep.h"
#include "peeps/peepHead.h"
#include "peeps/peepBody.h"
#include "assert.h"
#include "blood/bloodSpout.h"
#include "blood/bloodEngine.h"

#include <algorithm>

class PeepDriver::DeadPeep
{
public:
   // duration is in ticks, not seconds
   DeadPeep(PeepHead* head, BloodSpout& headSpout,
            PeepBody* body, BloodSpout& bodySpout,
            CHUint duration) :
      m_peepHead(head), 
      m_headSpout(headSpout), 
      m_peepBody(body), 
      m_bodySpout(bodySpout),
      m_maxDuration(duration), m_duration(0)
   {
      assert( m_peepBody != NULL && m_peepHead != NULL );   
   }
   ~DeadPeep()
   {
      m_bodySpout.stop();
      m_headSpout.stop();
      delete m_peepBody;
      delete m_peepHead;
   }
   void render()
   {
      m_peepHead->renderDead();
      m_headSpout.render();
      m_peepBody->renderDead();
      m_bodySpout.render();
   }
   void process()
   {
      ++m_duration;

      m_peepBody->integrate(0.25);
      m_bodySpout.setPosition(m_peepBody->position());

      m_peepHead->integrate(0.25);
      m_headSpout.setPosition(m_peepHead->position());

         // rotations?
   }
   bool durationExceeded() const {return m_duration >= m_maxDuration;}
private:
   CHUint m_maxDuration;
   CHUint m_duration;
   PeepBody* m_peepBody;
   PeepHead* m_peepHead;
   BloodSpout& m_bodySpout;
   BloodSpout& m_headSpout;
};

PeepDriver::PeepDriver(BloodEngine& bloodEngine) :
   m_peeps(),
   m_peepParts(),
   m_maxPeeps(MAX_PEEPS),
   m_bloodEngine(bloodEngine)
{}

PeepDriver::~PeepDriver()
{
   CHUint sz = (CHUint)m_peeps.size();
   for( CHUint i=0; i<sz; ++i )
      delete m_peeps[i];
   m_peeps.clear();

   sz = (CHUint)m_peepParts.size();
   for( CHUint i=0; i<sz; ++i )
      delete m_peepParts[i];
   m_peepParts.clear();   
}

void PeepDriver::initPeeps()
{
}

BloodSpout& PeepDriver::createBloodSpoutForBodyPart(const AbstractPeep& peep)
{   
   Vector3 spurtRotation = peep.rotation();
   Vector3 spurtPosition = peep.position();
   Vector3 spurtOrientation = peep.orientation();

   if(peep.peepType() == TypePeepHead)
      spurtOrientation.invert();

   BloodSpout* spout = 
      m_bloodEngine.addSpout(spurtPosition, 
                             spurtOrientation, 
                             spurtRotation,
                             BLOOD_SPURT_DENSITY,
                             1.5, BLOOD_DROPLET_SIZE, BLOOD_DROPLET_DURATION );
   return *spout;
}

void PeepDriver::renderPeeps() const
{
   PEEPS::const_iterator curPeep = m_peeps.begin();
   while( curPeep != m_peeps.end() )
   {
      Peep* peep = (*curPeep);
      (*curPeep)->render();
      ++curPeep;
   }
   renderParts();
}
void PeepDriver::renderParts() const
{
   PEEP_PARTS::const_iterator curPart = m_peepParts.begin();
   while( curPart != m_peepParts.end() )
   {
      (*curPart)->render();
      ++curPart;
   }
}

void PeepDriver::processParts()
{
   CHUint sz = (CHUint)m_peepParts.size();
   for( CHUint i = 0; i<sz; ++i )
   {
      m_peepParts[i]->process();
      if(m_peepParts[i]->durationExceeded())
      {
         delete m_peepParts[i];
         m_peepParts.erase( m_peepParts.begin() + i );
         sz = (CHUint)m_peepParts.size();
         --i;
      }
   }
}

static bool peepZOrder ( const Peep* elem1, const Peep* elem2 )
{
   return elem1->position().z < elem2->position().z;
}

static void sortPeepsForRendering(PeepDriver::PEEPS& peeps)
{
   std::sort( peeps.begin(), peeps.end(), peepZOrder );
}

void PeepDriver::process()
{
   Vector3 startOrientation = PEEP_START_ORIENTATION;
   startOrientation.normalise();

   while( (CHUint)m_peeps.size() < (CHUint)m_maxPeeps )
   {
      Vector3 orientation = utils3D::randomizeUnitVec(startOrientation, -PIOver4);

      // peeps can't float
      orientation.y = 0.0f;
      CHUint tmp = rand() %2;
      if(tmp == 0)
         orientation.invert();

      m_peeps.push_back(new Peep(PEEP_START_POSITION, orientation, PEEP_START_ROTATION, PEEP_SCALE));
   }

   CHUint sz = (CHUint)m_peeps.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if(m_peeps[i]->finishedThrashing())
      {
         PeepHead* head = NULL;
         PeepBody* body = NULL;
         m_peeps[i]->Split(head, body);

         Vector3 orientation( m_peeps[i]->orientation() );

         Vector3 vec( utils3D::randomizeUnitVec(orientation, PIOver6) );
         head->setVelocity( m_peeps[i]->velocity() );
         head->setOrientation(vec);
         head->setEyeScale(DEAD_PEEP_EYESCALE);
         head->setMouthScale(DEAD_PEEP_MOUTHSCALE);
         head->setMass(2.0f);
         head->setDamping(0.8f);
         head->setAcceleration(GRAVITY);
         head->clearAccumulator();

         vec = utils3D::randomizeUnitVec(orientation, PIOver6);
         body->setVelocity( m_peeps[i]->velocity() );
         body->setOrientation(vec);
         body->setMass(2.0f);
         body->setDamping(0.8f);
         body->setAcceleration(GRAVITY);
         body->clearAccumulator();

         m_peepParts.push_back( 
            new DeadPeep(head, 
                         createBloodSpoutForBodyPart(*head), 
                         body, 
                         createBloodSpoutForBodyPart(*body),
                         DEAD_PEEP_DURATION));     

         delete m_peeps[i];
         m_peeps.erase( m_peeps.begin() + i );
         sz = (CHUint)m_peeps.size();
         --i; 
      }         
   }

   sz = (CHUint)m_peeps.size();
   for( CHUint i=0; i<sz; ++i )
   {
#ifdef PEEPS_MOVING
      if( ! m_peeps[i]->isThrashing())
      {
         m_peeps[i]->move();
         m_peeps[i]->animate();
      }
#endif
   }

   processParts();
   
   sortPeepsForRendering(m_peeps);
}


CHUint PeepDriver::numPeepParts() const
{
   return (CHUint)m_peepParts.size();
}