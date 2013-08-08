#include "cdhBase.h"
#include "StarSystem.h"
#include "../CollidingParticle.h"
#include "Infrastructure/World.h"


static MyParticleActor* 
CreateNewParticleCluster(float posx, float posy)
{   
	// Create the particle actor via the Actor Definition system (.adf files)
	MyParticleActor* pa = new MyParticleActor();
   pa->SetPosition(posx,posy);
	pa->SetColor(1.0f, 1.0f, 1.0f);  //Sets the initial color of the particles. 
									 // Since the image file we'll be using already
									 // has a color, we set this to pure white. 
	
	pa->SetSize(Vector2(0.2f, 0.2f)); //The size of each particle, in GL units
   
	pa->LoadSpriteFrames("Resources/Images/Star/particle_01.png");
	pa->PlaySpriteAnimation(0.1f, SAT_Loop, 0, 10);

	pa->SetParticleLifetime(12.0f); //How long each particles lasts before disappearing
	pa->SetSpread(MathUtil::Pi); //The angle in radians at which particles will be emitted.
	pa->SetSpeedRange(4, 7); //The minimum and maximum range of speeds (so you can have
								   // some variation).

   pa->generateCluster(5);
   return pa;
}


static const float StarTwinkleTime = 1.6f;
static const float StarDecayTime = 1.0f;
static const float StarExplodeTime = 0.1f;

Star::Star() :
   m_state(Twinkle),
   m_particleCluster(NULL),
   m_graphic(NULL),
   m_timeExistant(0.0f)
{   
   float posx = MathUtil::RandomFloatInRange(-10, 10);
   float posy = MathUtil::RandomFloatInRange(-10, 10);
   m_particleCluster = CreateNewParticleCluster(posx, posy);
   m_graphic = new Actor();
   m_graphic->SetPosition(posx,posy);
   m_graphic->SetSize(1.0f);
   theWorld.Add(m_graphic,10);
   
	m_graphic->LoadSpriteFrames("Resources/Images/Star/star_01.png");
	m_graphic->PlaySpriteAnimation(0.1f, SAT_Loop, 0, 10);
}
Star::~Star()
{
   delete m_particleCluster; m_particleCluster = NULL;
   delete m_graphic; m_graphic = NULL;
}
void Star::Destroy()
{
   m_state = Destroyed;
   theWorld.Remove(m_particleCluster);
   theWorld.Remove(m_graphic);
   m_particleCluster->Destroy();
   m_graphic->Destroy();
}
void Star::Update(float dt)
{
   m_timeExistant += dt;
   switch(m_state)
   {
   case Twinkle:
      if(m_timeExistant >= StarTwinkleTime)
      {
         m_timeExistant = 0.0f;
         m_state = Decay;
         
         m_graphic->LoadSpriteFrames("Resources/Images/Star/enmstar_01.png");
         m_graphic->PlaySpriteAnimation(0.1f, SAT_Loop, 0, 10);
      }
      break;
   case Decay: 
      if(m_timeExistant >= StarDecayTime)
      {
         m_timeExistant = 0.0f;
         m_graphic->Destroy();
         theWorld.Add(m_particleCluster,9);
         m_state = Exploding;
      }
      break;
   case Exploding: 
      if(m_timeExistant >= StarExplodeTime)
      {
         m_timeExistant = 0.0f;
      }
      break;
   case Destroyed:
      break;
   default: assert(!"Shouldn't get here");
   }
}
static const float StarSpawnRate = 6.0f;
static const float StarExistTime = StarTwinkleTime + StarDecayTime + 2.0f;
static float g_spawningTime = 0.0f;

StarSystem::StarSystem() :
   m_stars(),
   m_destroyedStars()
{
}

void StarSystem::GetStars(std::vector<const MyParticleActor*>& stars) const
{
   StarMap::const_iterator iter = m_stars.begin();
   while(iter != m_stars.end())
   {
      stars.push_back(iter->first->particleCluster());
      ++iter;
   }
}

void StarSystem::Cleanup()
{
   size_t sz = m_destroyedStars.size();
   for( size_t i=0; i<sz; ++i)
   {
      m_stars.erase( m_destroyedStars[i] );
   }
   m_destroyedStars.clear();
}
void StarSystem::Update(float dt)
{
   StarMap::iterator iter = m_stars.begin();
   while(iter != m_stars.end())
   {
      iter->second += dt;
      iter->first->Update(dt);
      if(iter->second >= StarExistTime)
      {
         iter->first->Destroy();
         m_destroyedStars.push_back(iter->first);
      }
      ++iter;
   }

   Cleanup();

   g_spawningTime += dt;
   if(g_spawningTime >= StarSpawnRate)
   {
      g_spawningTime = 0.0f;
      Star* star = new Star();
      m_stars[star] = 0.0f;
   }
}