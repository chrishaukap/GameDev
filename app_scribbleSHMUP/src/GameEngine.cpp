#include "GameEngine.h"

#include <assert.h>
#include "camera/camera.h"
#include <vector>
#include <float.h>

#include "math/vector3D.h"
#include "defines.h"

#include "objectScroller.h"
#include "oggController.h"
#include "oggPlayer.h"
#include "mainCharacter.h"
#include "scribbleEnemy.h"
#include "actor/foregroundObject.h"
#include "actor/projectile.h"
#include "actor/gameObject.h"
#include "sprite/animation.h"
#include "sprite/sprite.h"
#include "sprite/background.h"
#include "resource/textureManager.h"
#include "resource/resourceLibrary.h"
#include "actor/collidable.h"
#include "levelObjects.h"

#include "time.h"

const char g_strBackdrop[] = "images/backdrop.bmp";

using namespace CDH;
static RES_HANDLE g_backdrop = 0;
static OGG_HANDLE g_bgMusicHandle = 0;
static OGG_HANDLE g_bossMusicHandle = 0;
static std::vector<Projectile*> g_projectiles;

// HAUKAP - MEGA HACK
static Level1Boss* g_boss = NULL;


GameEngine::GameEngine(const KeyInput& input) :
   m_camera(NULL),
   m_input(input),
	m_levelObjects(NULL),
	m_mainCharacter(NULL),
	m_background(NULL),
	m_projectileAnimation(NULL),
	m_mainCharacterAnimation(NULL),
   m_objectScroller(NULL),
	m_boundaryLeft(0.0f),
   m_boundaryRight(0.0f)
{   
   srand ( (CHUint)time(NULL) );
}

GameEngine::~GameEngine()
{
	CHUint sz = (CHUint)g_projectiles.size();
	for(CHUint i=0; i<sz; ++i)
		delete g_projectiles[i];
	g_projectiles.clear();
   destroy();
}

void GameEngine::destroy()
{
	Status stat = ResourceLibrary::terminate();
   assert(StatusSucceeded(stat));

   stat = TextureManager::terminate();
   assert(StatusSucceeded(stat));

   stat = OggController::oggTerminate();
   assert(StatusSucceeded(stat));

   delete m_camera;
	m_camera = NULL;

	delete m_levelObjects;
	m_levelObjects = NULL;

	delete m_mainCharacter;
	m_mainCharacter = NULL;

	delete m_background;
	m_background = NULL;

	delete m_projectileAnimation;
	m_projectileAnimation = NULL;

	delete m_mainCharacterAnimation;
	m_mainCharacterAnimation = NULL;

   delete m_objectScroller;
   m_objectScroller = NULL;
}

void GameEngine::initializePreMenu()
{
   Status stat = OggController::oggInitialize();   
   assert(stat == StatusSuccess);
	
   OGG_HANDLE handle = OggController::loadOggFile("sound\\HeroHit.ogg");
   MainCharacter::setHitSound(handle);
   handle = OggController::loadOggFile("sound\\HeroJump.ogg");
   OggController::getOggPlayer(handle)->setVolume(6.0f);
   MainCharacter::setJumpSound(handle);
   handle = OggController::loadOggFile("sound\\HeroDeath.ogg");
   MainCharacter::setDeathSound(handle);
   handle = OggController::loadOggFile("sound\\HeroHurt1.ogg");
   OGG_HANDLE h2 = OggController::loadOggFile("sound\\HeroHurt2.ogg");
   MainCharacter::setHurtSounds(handle, h2);
   handle = OggController::loadOggFile("sound\\HeroPickup.ogg");
   MainCharacter::setPickupSound(handle);
	
	stat = ResourceLibrary::initialize();
   assert(stat == StatusSuccess);

	m_levelObjects = new LevelObjects( *this, 0.0f, 0.0f, 40.0f, 40.0f);
	m_levelObjects->load();
	m_boundaryLeft = -14.75f;
   m_boundaryRight = 12.25f;

	// HAUKAP - move to "level loader"
   g_bgMusicHandle = OggController::loadOggFile( "sound\\1.ogg" );
   OggController::getOggPlayer(g_bgMusicHandle)->setLooping(true);
   OggController::getOggPlayer(g_bgMusicHandle)->setVolume(0.15f);

   g_bossMusicHandle = OggController::loadOggFile( "sound\\2.ogg" );
   OggController::getOggPlayer(g_bossMusicHandle)->setLooping(true);
   OggController::getOggPlayer(g_bossMusicHandle)->setVolume(0.15f);


   m_camera = new Camera(Vector3(0.0f, 0.0f, 35.0f));
	
   stat = ResourceLibrary::LoadResource(g_strBackdrop, g_backdrop);
	
	RES_HANDLE  r1, r2, r3, r4, r5, r6, r7, r8,
               r9, r10, r11, r12, r13, r14, r15, r16,
					r31, r32;
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame1_mask.bmp", r1);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame2_mask.bmp", r2);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame3_mask.bmp", r3);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame4_mask.bmp", r4);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame5_mask.bmp", r5);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame6_mask.bmp", r6);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame7_mask.bmp", r7);
	stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame8_mask.bmp", r8);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame1.bmp", r9);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame2.bmp", r10);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame3.bmp", r11);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame4.bmp", r12);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame5.bmp", r13);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame6.bmp", r14);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame7.bmp", r15);
   stat = ResourceLibrary::LoadResource("images/walkcycle_right_frame8.bmp", r16);
   stat = ResourceLibrary::LoadResource("images/bullet_mask.bmp", r31);
   stat = ResourceLibrary::LoadResource("images/bullet.bmp", r32);
	
	stat = TextureManager::initialize();

	m_projectileAnimation = new Animation(3);
	m_projectileAnimation->pushFrameAtEnd(Frame(r32, r31));
	
	// init main character
	m_mainCharacterAnimation = new Animation(3);
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r9, r1));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r10, r2));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r11, r3));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r12, r4));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r13, r5));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r14, r6));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r15, r7));
	m_mainCharacterAnimation->pushFrameAtEnd(Frame(r16, r8));

   const char mainCharAnimWalkRight[] = "walkRight";
	m_mainCharacter = 
		new MainCharacter(*this, 
								m_input, 
                        MainCharacter::StartingLocation, 
								Vector3(0.0f, 0.0f, 0.0f),
								CDH::Rectangle(3.0f, 6.0f));
	m_mainCharacter->sprite()->addAnimation( mainCharAnimWalkRight, m_mainCharacterAnimation );
	m_mainCharacter->sprite()->setCurrentAnimation( mainCharAnimWalkRight );

	// init background
	m_background = new Background();
	m_background->setBackground(g_backdrop, -20.8f, -15.0f, 0.0025f);

   const float scrollRate = 0.15f;
   m_objectScroller = new ObjectScroller;
   m_objectScroller->setDirectionToScroll(ObjectScroller::Left);
   m_objectScroller->setScrollRate(scrollRate);
   m_objectScroller->addObject( m_mainCharacter );

}

void GameEngine::initializePostMenu()
{
   OggPlayer* bgMusic = OggController::getOggPlayer(g_bgMusicHandle);
   bgMusic->play();

	m_levelObjects->startLevel();

	GameObjects objects = m_levelObjects->getAllObjects();
	CHUint sz = (CHUint)objects.size();
	for( CHUint i=0; i<sz; ++i )
      m_objectScroller->addObject( objects[i] );   
}

void GameEngine::render() const
{	
	// HAUKAP - game engine shouldn't be rendering.  
	//				CGfxOpenGL::render() should take a list of renderables and render them
	
	Status stat = m_background->render();
	assert(StatusSucceeded(stat));

	GameObjects objects = m_levelObjects->getObjectsInPortal();
	CHUint sz = (CHUint)objects.size();
	for( CHUint i=0; i<sz; ++i )
   {
		stat = objects[i]->render();
      assert(StatusSucceeded(stat));
   }

	stat = m_mainCharacter->render();
	assert(StatusSucceeded(stat));

	sz = (CHUint)g_projectiles.size();
	for( CHUint i=0; i<sz; ++i )
   {
		stat = g_projectiles[i]->render();
      assert(StatusSucceeded(stat));
   }   
}

// HAUKAP - another hack
static bool g_bossExists = false;
CHint
GameEngine::getEndBossHitPoints() const
{
   if(g_boss != NULL)
   {
      g_bossExists = true;
      return g_boss->hitpoints();
   }
   return -1;
}
bool 
GameEngine::bossKilled() const
{
   return (g_bossExists && g_boss->hitpoints() <= 0);
}

void GameEngine::processCamera()
{
   assert(m_camera != NULL);
   m_camera->process();
}

static bool bossMusicPlaying = false;
void GameEngine::process()
{   
	m_levelObjects->process();

   processCamera();
	Status stat = m_mainCharacter->process();
	assert(StatusSucceeded(stat));
   
	GameObjects objects = m_levelObjects->getObjectsInPortal();
	CHUint sz = (CHUint)objects.size();
	for( CHUint i=0; i<sz; ++i )
   {
      if(objects[i]->isEnemy())
      {
         ScribbleEnemy* enemy = dynamic_cast<ScribbleEnemy*>( objects[i] );
         if(enemy->isBoss())
         {
            g_boss = dynamic_cast<Level1Boss*>( enemy );
         }
      }

		stat = objects[i]->process();
      assert(StatusSucceeded(stat));
   }

   if(!bossMusicPlaying && g_boss != NULL)
   {
      OggController::getOggPlayer(g_bgMusicHandle)->stop();
      OggController::getOggPlayer(g_bossMusicHandle)->play();
      bossMusicPlaying = true;
   }

	sz = (CHUint)g_projectiles.size();
	for( CHUint i=0; i<sz; ++i )
   {
		stat = g_projectiles[i]->process();
      assert(StatusSucceeded(stat));
   }
	
   resolveCollisions();
   objectCleanup();

   m_objectScroller->process();

	// ensure Hero stays on screen
	Vector3 mainPos(m_mainCharacter->position());
	if(mainPos.x < m_boundaryLeft)
	{
		mainPos.x = m_boundaryLeft;
		m_mainCharacter->setPosition(mainPos); 
	}
   else if( mainPos.x > m_boundaryRight )
   {
      mainPos.x = m_boundaryRight;
      m_mainCharacter->setPosition(mainPos);
   }
   if(mainPos.y < -12.0f)
   {
      m_mainCharacter->setVelocity( Vector3(0.0f, 0.0f, 0.0f) );
      m_mainCharacter->setPosition( mainPos.x, -12.0f, 0.0f);
   }
}
   
Status
GameEngine::createProjectile(ProjectileType type, 
									  const Vector3& position, 
									  const Vector3& orientation,
                             CHUint team)
{
	Vector3 rotation(0.0f, 0.0f, 0.0f);
	Projectile* projectile = NULL;
	switch(type)
	{
	case Throwing: 
		projectile = new Projectile(position, rotation, orientation, 6.0, 1.0f, CDH::Rectangle(1.0f, 1.0f), team);
		projectile->sprite()->addAnimation( "ThrowingWpn", m_projectileAnimation );

      assert( orientation.x != 0 );
      projectile->setFacing( orientation.x > 0 ? GameObject::FacingRight : GameObject::FacingLeft );
		break;
	case Rocket:
		projectile = new Projectile(position, rotation, orientation, 9.0, 1.0f, CDH::Rectangle(1.0f, 1.0f), team);	
		assert( !"not implemented");
		break;
	default:
		assert( !"Invalid ProjectileType" );
	}
	g_projectiles.push_back(projectile);

	return StatusSuccess;
}
void
GameEngine::objectCleanup()
{
   if(m_mainCharacter->isMarkedForRemoval())
   {
      if(m_mainCharacter->canRespawn())
      {
         m_mainCharacter->respawn();
         m_mainCharacter->decrementLife();
      }
      else
		{
			m_levelObjects->restartLevel();
         m_objectScroller->clear();
         
	      GameObjects objects = m_levelObjects->getAllObjects();
	      CHUint sz = (CHUint)objects.size();
	      for( CHUint i=0; i<sz; ++i )
            m_objectScroller->addObject( objects[i] );   

         m_mainCharacter->gainLives(3);
         m_mainCharacter->respawn();
		}
   }

   for( std::vector<Projectile*>::iterator iter = g_projectiles.begin();
        iter != g_projectiles.end();
        ++ iter )
	{
		if((*iter)->isMarkedForRemoval())
		{
         g_projectiles.erase(iter);
         if( g_projectiles.empty() )
            break;
         else
            iter = g_projectiles.begin();
		}
	}
}
void 
GameEngine::resolveCollisions()
{
	std::vector<AICharacter*> enemies;
	std::vector<ForegroundObject*> terrain;
	
	GameObjects objects = m_levelObjects->getObjectsInPortal();
	CHUint sz = (CHUint)objects.size();
	for(CHUint i=0; i<sz; ++i)
	{
		if(objects[i]->collidableType() == Collidable::CollidableTerrain)
			terrain.push_back(dynamic_cast<ForegroundObject*>(objects[i]));
		else if(objects[i]->collidableType() == Collidable::CollidableCharacter)
			enemies.push_back(dynamic_cast<AICharacter*>(objects[i]));
	}

	CHUint numTerrain = (CHUint)terrain.size();
	for(CHUint i=0; i<numTerrain; ++i)
	{
		if(m_mainCharacter->collidedWith( *(terrain[i]) ) )
		{
			m_mainCharacter->resolveCollision( *(terrain[i]) );
			terrain[i]->resolveCollision( *m_mainCharacter );
		}

      CHUint numEnemies = (CHUint)enemies.size();
      for(CHUint j=0; j<numEnemies; ++j)
      {
		   if(terrain[i]->collidedWith( *(enemies[j]) ))
		   {
			   terrain[i]->resolveCollision( *(enemies[j]) );
			   enemies[j]->resolveCollision( *(terrain[i]) );
		   }
      }		

		CHUint numProjectiles = (CHUint)g_projectiles.size();
      for(CHUint j=0; j<numProjectiles; ++j)
      {
         if(g_projectiles[j]->collidedWith( *m_mainCharacter ))
		   {
			   g_projectiles[j]->resolveCollision( *m_mainCharacter );
			   m_mainCharacter->resolveCollision( *(g_projectiles[j]) );
		   }
      }		
	}

	CHUint numEnemies = (CHUint)enemies.size();
	for(CHUint i=0; i<numEnemies; ++i )
	{
		if(m_mainCharacter->collidedWith( *(enemies[i]) ) )
		{
			m_mainCharacter->resolveCollision( *(enemies[i]) );
			enemies[i]->resolveCollision( *m_mainCharacter );
		}
	

		CHUint numProjectiles = (CHUint)g_projectiles.size();
      for(CHUint j=0; j<numProjectiles; ++j)
      {
		   if(g_projectiles[j]->collidedWith( *(enemies[i]) ))
		   {
			   g_projectiles[j]->resolveCollision( *(enemies[i]) );
			   enemies[i]->resolveCollision( *(g_projectiles[j]) );
		   }
      }		
	}


}