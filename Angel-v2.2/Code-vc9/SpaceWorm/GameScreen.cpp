#include "stdafx.h"
#include "cdhBase.h"
#include "GameScreen.h"
#include "Planet.h"
#include "Engine/Worm.h"
#include "BasicFastDispatcher.h"
#include "collisions.h"
#include "Engine/StarSystem.h"
#include "CollidingParticle.h"
#include "iCollidable.h"

namespace 
{
   BasicFastDispatcher<iCollidable> g_basicFastDispatcher;
   bool g_initted = false;
   const int PlanetLayer = 1;
   const int NumWormSegments = 10;
   static std::vector<Planet*> s_planets;
   const int NumPlanets = 4;
}
GameScreen* GameScreen::s_GameScreen = NULL;

GameScreen& GameScreen::GetInstance()
{
	if (s_GameScreen == NULL)
	{
		s_GameScreen = new GameScreen();
	}
	return *s_GameScreen;
}

GameScreen::GameScreen() :
   m_starSystem(NULL),
   m_background(NULL),
   m_worm(NULL)
{
   if(!g_initted)
   {
      g_initted = true;
		g_basicFastDispatcher.Add<CollidingParticle,Planet>(CollisionParticlePlanet);
      g_basicFastDispatcher.Add<CollidingParticle,Worm>(CollisionParticleWorm);
   }
}

void GameScreen::Start()
{
   m_background = new FullScreenActor();
   m_background->SetSprite("Resources/Images/Background.png");
   theWorld.Add(m_background, -1);

   for( int i =0; i< NumPlanets; ++i)
   {
      Planet *p = new Planet();
      s_planets.push_back(p);
      theWorld.Add(p,PlanetLayer);
   }
   
	m_starSystem = new StarSystem();

   m_worm = new Worm();
   m_worm->Init();
   for( int i=0;i < NumWormSegments; ++i)
      m_worm->GrowTail();
}
void GameScreen::Stop()
{
   int numPlanets = (int)s_planets.size();
   for( int i=0; i<numPlanets; ++i)
      delete( s_planets[i] );
   s_planets.clear();

   delete m_worm; m_worm = NULL;
   delete m_starSystem; m_starSystem = NULL;
}
Worm& 
GameScreen::worm() 
{
   return *m_worm;
}
const Worm& 
GameScreen::worm() const
{
   return *m_worm;
}
void GameScreen::Cleanup()
{
   m_starSystem->Cleanup();
}
void GameScreen::Update(float dt)
{
   m_worm->Update(dt);

   // collision detection/resolution
	const Vector2 WormPos = m_worm->GetPosition();
	const Vector2 WormSize = m_worm->GetSize();

   m_starSystem->Update(dt);
   std::vector<const ParticleActor*> stars;
   m_starSystem->GetStars(stars);
   size_t numStars = stars.size();
   for(size_t i=0; i<numStars; ++i)
   {
      std::vector<Particle*> particles;
      stars[i]->GetParticles(particles);

      int numParticles = (int)particles.size();
      //assert(numParticles==25);
	   for( int i=0; i<numParticles; ++i)
      {
         const Vector2 particlePos = particles[i]->_pos;
         const Vector2 particleSize = particles[i]->_scale;
         Point<float> ul(particlePos.X - particleSize.X * 0.5f, particlePos.Y - particleSize.Y * 0.5f);
         Point<float> lr(particlePos.X + particleSize.X * 0.5f, particlePos.Y + particleSize.Y * 0.5f);
         Rect<float> particleRect(ul,lr);
         CollidingParticle* particle = dynamic_cast<CollidingParticle*>(particles[i]);

         for( int j=0; j<NumPlanets; ++j)
         {
            const Vector2 planetPos = s_planets[j]->GetPosition();
		      const Vector2 planetSize = s_planets[j]->GetSize();
            Point<float> ul(planetPos.X - planetSize.X * 0.5f, planetPos.Y - planetSize.Y * 0.5f);
            Point<float> lr(planetPos.X + planetSize.X * 0.5f, planetPos.Y + planetSize.Y * 0.5f);
            Rect<float> planetRect(ul,lr);
            if(RectanglesIntersect(particleRect, planetRect))
            {
               g_basicFastDispatcher.Go(*particle, *(s_planets[j]));
		      }
         }

         if(m_worm->IntersectsWithRect(particleRect))
         {
            g_basicFastDispatcher.Go(*particle, *m_worm);
		   }
      }
   }

	/*//This is the same kind of input processing we did in DemoScreenSimpleActor.cpp,
	//  but here we're playing with various properties of the particles. 
	if ((theController.IsConnected() && theController.IsBButtonDown()) || theInput.IsKeyDown('b'))
	{
		pa->SetColor(1.0f, 0.0f, 0.0f, .25f);
		pa->SetSize(0.5f, 0.5f);
		pa->SetGravity( Vector2::Zero );
		pa->ClearSpriteInfo();
		t->SetDisplayString("Now it's red and translucent. Press [Y].");
		t2->SetDisplayString("");
	}
	if ((theController.IsConnected() && theController.IsYButtonDown()) || theInput.IsKeyDown('y'))
	{
		pa->SetColor(0,0,1,1); 		
		pa->SetSize(0.5f, 0.5f);
		Vector2 grav = Vector2(0.0f, -8.0f);
		pa->SetGravity( grav );
		pa->ClearSpriteInfo();
		t->SetDisplayString("Now it's blue and has density. Press [X] for the coolest thing ever.");
		t2->SetDisplayString("");
	}
	if ((theController.IsConnected() && theController.IsXButtonDown()) || theInput.IsKeyDown('x'))
	{
		pa->SetSize(4.0f, 4.0f);
		pa->SetColor(1,1,1,1);
		pa->SetGravity( Vector2::Zero );
		pa->LoadSpriteFrames("Resources/Images/numbers/angel_01.png");
		pa->PlaySpriteAnimation(0.5f, SAT_Loop, 0, 4);

		t->SetDisplayString("That's right, animated textures. You love it.");
		t2->SetDisplayString("");
	}*/
}
void GameScreen::UpdatePlanetSelections(Planet* planet)
{
   int numPlanets = (int)s_planets.size();
   for( int i=0; i<numPlanets; ++i)
      s_planets[i]->Select(  s_planets[i] == planet );
}

bool GameScreen::ClickedOnPlanet(const Vec2i& screenCoords, Planet*& planet) const
{
   planet = NULL;
   Vector2 clickPos = MathUtil::ScreenToWorld(screenCoords);

   int numPlanets = (int)s_planets.size();
   for( int i=0; i<numPlanets; ++i)
   {
      const Vector2 pos = s_planets[i]->GetPosition();
      const Vector2 sz  = s_planets[i]->GetSize();

      Point<float> ul(pos.X - sz.X * 0.5f, pos.Y - sz.Y * 0.5f);
      Point<float> lr(pos.X + sz.X * 0.5f, pos.Y + sz.Y * 0.5f);
      if(PointInRect(Point<float>(clickPos.X, clickPos.Y),
                     Rect<float>(ul,lr)))
      {
         planet = s_planets[i];
         return true;
      }
   }
   return false;
}

void GameScreen::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
   Planet* planet = NULL;
   bool planetClicked = ClickedOnPlanet(screenCoords, planet);

   if(planetClicked)
   {
      if(!planet->IsSelected())
      {
         planet->SetColor(0,1,0,1);   
         m_worm->SetPlanetOrbit(planet);  
      }
   }
   else     
      m_worm->SetPlanetOrbit(NULL); 

   UpdatePlanetSelections(planet);
}

void GameScreen::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
}