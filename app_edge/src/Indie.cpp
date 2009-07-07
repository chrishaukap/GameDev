
#include "Indie.h"
#include "MathUtil.h"
#include <assert.h>

// HAUKAP - for cheezy collision
#include "Tim.h"

using namespace CDH;
using namespace Edge;

Indie::Indie() : 
   m_state(eLiving),
   m_dyingCounter(0)
{}
Indie::~Indie()
{}

void Indie::Collided(const Tim*)
{
   if(!isDead())
   {
	   m_state = eDying;
	   SetColor( 0.0f, 1.0f, 0.0f );
   }
}
void Indie::Collided(const Indie*)
{
}

void Indie::Update(float dt)
{
   switch(m_state)
   {
   case eLiving:
      {
	      const float dPos = 0;	
	      SetPosition( _position.X + dt * MathUtil::randomizeValue(dPos, 5.0f),
					       _position.Y + dt * MathUtil::randomizeValue(dPos, 5.0f));
      }
      break;     
   case eDying:
      if( ++m_dyingCounter > 2000 )
         m_state = eDead;
      break;
   case eDead:
      break;;
   case eDeleteMe:
      break;
   default:
      assert(!"Invalid Indie State");
   }
}