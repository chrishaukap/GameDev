
#include "Game.h"
#include "Indie.h"
#include "Money.h"
#include "Tim.h"
#include "Infrastructure/World.h"
#include "Util/MathUtil.h"
#include "BasicFastDispatcher.h"
#include "collisions.h"

#include <assert.h>
using namespace Edge;
#define NUM_INDIES 50
#define NUM_MONEYS 8

namespace 
{
   BasicFastDispatcher<iCollidable> g_basicFastDispatcher;
   bool g_initted = false;
}

Game::Game(unsigned int rows, unsigned int cols) :
	m_indies(),
	m_moneys(),
   m_tim()
{
   if(!g_initted)
   {
      g_initted = true;
		g_basicFastDispatcher.Add<Indie,Indie>(CollisionIndieIndie);
		g_basicFastDispatcher.Add<Money,Indie>(CollisionMoneyIndie);
		g_basicFastDispatcher.Add<Tim,Indie>(CollisionTimIndie);
   }
}
Game::~Game()
{}
void 
Game::initMoney()
{
	for(unsigned int i=0; i< NUM_MONEYS; ++i)
	{
		Money* money = new Money();
		money->SetSize(0.7f);

      Vector2 pos = MathUtil::RandomVector(Vector2(-8,-8), Vector2(8,8));

      money->SetPosition(pos);
		theWorld.Add(money);
		m_moneys.push_back( money );
	}
}
void 
Game::initIndies()
{
	for(unsigned int  i=0; i< NUM_INDIES; ++i)
	{
		Indie* indie = new Indie();
		indie->SetSize(1.0f);

      Vector2 pos = MathUtil::RandomVector(Vector2(-8,-8), Vector2(8,8));

      indie->SetPosition(pos);
		indie->SetColor(0,0,0);
		theWorld.Add(indie,1);
		m_indies.push_back( indie );
	}
}
void 
Game::initTim()
{
   m_tim = new Tim();
   m_tim->Init();
}

void 
Game::start()
{
	initMoney();
	initIndies();
	initTim();
}
void 
Game::destroyMoney()
{
	MONEY_LIST::iterator iter1 = m_moneys.begin();
	while( iter1 != m_moneys.end() )
	{
		theWorld.Remove( *iter1 );
		delete (*iter1);
		++iter1;
	}
	m_moneys.clear();
}
void 
Game::destroyIndies()
{
	INDIE_LIST::iterator iter = m_indies.begin();
	while( iter != m_indies.end() )
	{
		theWorld.Remove( *iter );
		delete (*iter);
		++iter;
	}
	m_indies.clear();
}
void 
Game::destroyTim()
{	
   delete m_tim; m_tim = NULL;
}
void 
Game::stop()
{
	destroyMoney();
	destroyIndies();
	destroyTim();
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
	INDIE_LIST::iterator iter = m_indies.begin();
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
	MONEY_LIST::iterator iter2 = m_moneys.begin();
	while( iter2 != m_moneys.end() )
	{
      if((*iter2)->isMarkedForDeletion())
      {
			theWorld.Remove((*iter2));
         (*iter2)->Destroy();
         delete (*iter2);
         iter2 = m_moneys.erase(iter2);
      }
      else
         ++iter2;
	}
}
void
Game::update(float dt)
{  
   // update game objects
	INDIE_LIST::iterator iter = m_indies.begin();
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
                             indiePos.X, timPos.X,
                             indiePos.X + indieSize.X, timPos.X + timSize.X))
      {
         g_basicFastDispatcher.Go(*m_tim, *(*iter));
		}

		MONEY_LIST::iterator moneyIter = m_moneys.begin();
		while( moneyIter != m_moneys.end() )
		{
			if(RectanglesIntersect(indiePos.Y, (*moneyIter)->GetPosition().Y,
										  indiePos.Y + indieSize.Y, (*moneyIter)->GetPosition().Y + (*moneyIter)->GetSize().Y,
										  indiePos.X, (*moneyIter)->GetPosition().X,
										  indiePos.X + indieSize.X, (*moneyIter)->GetPosition().X + (*moneyIter)->GetSize().X))
			{
            g_basicFastDispatcher.Go( *(*moneyIter), *(*iter));
			}

			++moneyIter;
		}

		INDIE_LIST::iterator indieIter2 = m_indies.begin();
		while( indieIter2 != m_indies.end() )
		{
			if((*indieIter2) == (*iter))
			{
				++indieIter2;
				continue;
			}

			const Vector2 indiePos = (*indieIter2)->GetPosition();
			const Vector2 indieSize = (*indieIter2)->GetSize();
			const Vector2 indiePos2 = (*iter)->GetPosition();
			const Vector2 indieSize2 = (*iter)->GetSize();
			if(RectanglesIntersect(indiePos.Y, indiePos2.Y,
										  indiePos.Y + indieSize.Y, indiePos2.Y + indieSize2.Y,
										  indiePos.X, indiePos2.X,
										  indiePos.X + indieSize.X, indiePos2.X + indieSize2.X))
			{
				g_basicFastDispatcher.Go(*(*indieIter2), *(*iter));
			}
			++indieIter2;
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

         // add Tail Nodes 
         int random = MathUtil::RandomIntInRange(1,10);
         if(random >= 7)
            m_tim->GrowTail();
      }
		++iter;
	}
}