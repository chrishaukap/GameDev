#include "Game.h"
#include "../Planet.h"
#include "Worm.h"
#include "../CollidingParticle.h"
#include "BasicFastDispatcher.h"
#include "../collisions.h"
#include "../iCollidable.h"
#include "Actors/FullScreenActor.h"
#include "../MyParticleActor.h"

namespace 
{
   BasicFastDispatcher<iCollidable> g_basicFastDispatcher;
   const int PlanetLayer = 1;
   const int NumWormSegments = 10;
   static std::vector<Planet*> s_planets;
   const int NumPlanets = 4;
}
SpaceWormGame* SpaceWormGame::s_SpaceWormGame = NULL;

SpaceWormGame& SpaceWormGame::GetInstance()
{
	if (s_SpaceWormGame == NULL)
	{
		s_SpaceWormGame = new SpaceWormGame();
      g_basicFastDispatcher.Add<CollidingParticle,Planet>(CollisionParticlePlanet);
      g_basicFastDispatcher.Add<CollidingParticle,Worm>(CollisionParticleWorm);
	}
	return *s_SpaceWormGame;
}
SpaceWormGame::SpaceWormGame() :
   m_background(NULL),
   m_planets(),
   m_worm(NULL)
{   
   m_background = new FullScreenActor();
   m_background->SetSprite("Resources/Images/Background.png");

   for( int i =0; i< NumPlanets; ++i)
   {
      Planet *p = new Planet();
      s_planets.push_back(p);
   }
   
   m_worm = new Worm();
   m_worm->Init();
   for( int i=0;i < NumWormSegments; ++i)
      m_worm->GrowTail();
}
SpaceWormGame::~SpaceWormGame()
{   
}

const Planets& SpaceWormGame::planets() const
{
   return m_planets;
}

Planets& SpaceWormGame::planets()
{
   return m_planets;
}
const Worm& SpaceWormGame::worm() const 
{
   return *m_worm;
}

const FullScreenActor& SpaceWormGame::background() const
{
   return *m_background;
}
FullScreenActor& SpaceWormGame::background()
{
   return *m_background;
}