#include "stdafx.h"
#include "cdhBase.h"
#include "Engine/Game.h"
#include "Planet.h"
#include "Engine/Worm.h"
#include "CollidingParticle.h"
#include "BasicFastDispatcher.h"
#include "collisions.h"
#include "iCollidable.h"

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
   m_starEngine(NULL)
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
   
	// Create the particle actor via the Actor Definition system (.adf files)
	m_starEngine = new ParticleActor();
	m_starEngine->SetColor(1.0f, 1.0f, 1.0f);  //Sets the initial color of the particles. 
									 // Since the image file we'll be using already
									 // has a color, we set this to pure white. 
	
	m_starEngine->SetSize(Vector2(0.2f, 0.2f)); //The size of each particle, in GL units
	m_starEngine->SetSprite("Resources/Images/WormHead.png"); //The image file we want to use (otherwise 
												// it'll just be colored squares).
   const int maxNumParticles = 25;
   std::vector<Particle*> particles;
   for(int i=0; i<maxNumParticles; ++i)
      particles.push_back(new CollidingParticle);

   m_starEngine->SetParticles(particles);
   m_starEngine->SetParticlesPerSecond(25); //Emission Rate
	m_starEngine->SetParticleLifetime(4.0f); //How long each particles lasts before disappearing
	m_starEngine->SetSpread(MathUtil::Pi); //The angle in radians at which particles will be emitted. 
	m_starEngine->SetEndScale(0.5f); //If you want the particles to change size over their lifeWormes
	Color endColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_starEngine->SetEndColor(endColor); //Our particles disappear over Worme
	m_starEngine->SetEndScale(0.5f);	
	m_starEngine->SetSpeedRange(4.0f, 7.0f); //The minimum and maximum range of speeds (so you can have
								   // some variation).
	m_starEngine->SetGravity(Vector2::Zero); //You can pull the particles in a particular direction (default is
								   // downwards, so zero it out if you need to).	

   m_Worm = new Worm();
   m_Worm->Init();
   for( int i=0;i < NumWormSegments; ++i)
      m_Worm->GrowTail();
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

const ParticleActor& SpaceWormGame::starEngine() const
{
   return *m_starEngine;
}
ParticleActor& SpaceWormGame::starEngine()
{
   return *m_starEngine;
}

const FullScreenActor& SpaceWormGame::background() const
{
   return *m_background;
}
FullScreenActor& SpaceWormGame::background()
{
   return *m_background;
}