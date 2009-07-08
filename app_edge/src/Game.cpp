
#include "Game.h"
#include "Indie.h"
#include "Tim.h"
#include "World.h"
#include "MathUtil.h"

#include <assert.h>
using namespace CDH;
using namespace Edge;
#define NUM_INDIES 1

Game::Game(CHUint rows, CHUint cols) :
	m_indies(),
   m_tim()
{}
Game::~Game()
{}

void 
Game::start()
{
	for(CDH::CHUint i=0; i< NUM_INDIES; ++i)
	{
		Indie* indie = new Indie();
		indie->SetSize(1.0f);

      Vector2 pos = MathUtil::RandomVector(Vector2(-3,-3), Vector2(3,3));

      indie->SetPosition(pos);
		indie->SetColor(0,0,0);
		theWorld.Add(indie);
		m_indies.push_back( indie );
	}
   m_tim = new Tim();
   m_tim->Init();
}
void 
Game::stop()
{
	std::vector<Indie*>::iterator iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
		theWorld.Remove( *iter );
		delete (*iter);
		++iter;
	}
	m_indies.clear();
   
   delete m_tim; m_tim = NULL;
}

static bool 
RectanglesIntersect(float top1, float top2,
                    float bottom1, float bottom2,
                    float left1, float left2,
                    float right1, float right2)
{
   if (bottom1 < top2) return false;
   if (top1 > bottom2) return false;
   if (right1 < left2) return false;
   if (left1 > right2) return false;   
   return true;
}

void Game::cleanup()
{ 
   // cleanup objects
	std::vector<Indie*>::iterator iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
      if((*iter)->isMarkedForDeletion())
      {
         (*iter)->Destroy();
         delete (*iter);
         iter = m_indies.erase(iter);
      }
      else
         ++iter;
	}
}
void
Game::update(float dt)
{  
   // updated game objects
	std::vector<Indie*>::iterator iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
		(*iter)->Update(dt);
		++iter;
	}
	m_tim->Update(dt);

   // collision detection/resolution
	const Vector2 timPos = m_tim->GetPosition();
	const Vector2 timSize = m_tim->GetSize();
	iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
		const Vector2 indiePos = (*iter)->GetPosition();
		const Vector2 indieSize = (*iter)->GetSize();
      if(RectanglesIntersect(indiePos.Y, timPos.Y,
                             indiePos.Y + indieSize.Y, timPos.Y + timSize.Y,
                             indiePos.X,timPos.X,
                             indiePos.X + indieSize.X, timPos.X + timSize.X))
      {
         (*iter)->Collided(m_tim);
         m_tim->Collided( (*iter) );
		}
		++iter;
	}

   // Mark expired indies for removal
   iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
      if( (*iter)->isDead() )
      {
         theWorld.Remove(*iter);
         (*iter)->markMeForDeletion();
      }
		++iter;
	}


}