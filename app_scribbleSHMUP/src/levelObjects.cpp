#include "levelObjects.h"

#include "actor/gameObject.h"
#include "scribbleEnemy.h"
#include "platform.h"
#include "resource/resourceLibrary.h"
#include "sprite/animation.h"
#include "oggController.h"
#include "oggPlayer.h"
#include "sprite/sprite.h"
#include <vector>
#include "gameEngine.h"

using namespace CDH;

Animation* g_scribbleManAnimation; 
Animation* g_platformAnimation;
Animation* g_scribbleBossAnimation;

struct LevelObjects::Data
{
	Data() : 
		objects()
	{}
	~Data(){clear();}
	void clear()
	{
		CHUint sz = (CHUint) objects.size();
		for( CHUint i=0; i<sz; ++i )
			delete objects[i];
		objects.clear();
	}
	GameObjects objects;
};
LevelObjects::LevelObjects(GameEngine& engine,
                           float portalCenterX, float portalCenterY, 
									float portalWidth, float portalHeight) : 
   m_engine(engine),
	m_portalCenterX(portalCenterX),
	m_portalCenterY(portalCenterY),
	m_portalWidth(portalWidth),
	m_portalHeight(portalHeight),
	m_data(new Data)
{

}
LevelObjects::~LevelObjects()
{
	delete g_scribbleManAnimation;
	delete g_platformAnimation;
   delete g_scribbleBossAnimation;
	delete m_data;
}

void
LevelObjects::restartLevel()
{
	endLevel();
	startLevel();
}
void
LevelObjects::startLevel()
{	
	for( CHUint i=2; i<20; ++i )
	{
      float locX = i * 15.0f;
		ScribbleEnemy* enemy1 = 
					new ScribbleEnemy(Vector3( locX, 12.0f, 0.0f), 
											Vector3(0.0f, 0.0f, 0.0f), 
											CDH::Rectangle(4.0f, 4.0f), 2);
		ScribbleEnemy* enemy2 = 
					new ScribbleEnemy(Vector3( locX + 1.0f, 12.0f, 0.0f), 
											Vector3(0.0f, 0.0f, 0.0f), 
											CDH::Rectangle(4.0f, 4.0f), 2);
		ScribbleEnemy* enemy3 = 
					new ScribbleEnemy(Vector3( locX + 2.0f, 12.0f, 0.0f), 
											Vector3(0.0f, 0.0f, 0.0f), 
											CDH::Rectangle(4.0f, 4.0f), 2);
		enemy1->sprite()->addAnimation("walk", g_scribbleManAnimation);
		enemy2->sprite()->addAnimation("walk", g_scribbleManAnimation);
		enemy3->sprite()->addAnimation("walk", g_scribbleManAnimation);
		m_data->objects.push_back(enemy1);
		m_data->objects.push_back(enemy2);
		m_data->objects.push_back(enemy3);
	}

	for( CHUint i=1; i<10; ++i )
	{
      ScribbleBoss *boss = 
			new ScribbleBoss(Vector3( i * 25.0f, 12.0f, 0.0f), 
								  Vector3(0.0f, 0.0f, 0.0f), 
								  CDH::Rectangle(9.0f, 18.0f), 20);
      boss->sprite()->addAnimation("walk", g_scribbleManAnimation);
      m_data->objects.push_back( boss );
	}

   Level1Boss *endBoss = 
      new Level1Boss(Vector3( 300.0f, 12.0f, 0.0f), 
                     Vector3(0.0f, 0.0f, 0.0f), 
                     CDH::Rectangle(6.0f, 10.0f), 
                     m_engine);
   endBoss->sprite()->addAnimation("walk", g_scribbleBossAnimation);
   m_data->objects.push_back( endBoss );


	// load terrain objects
	{
		const float LedgeHeightLevel2  = -6.5f;
		const float LedgeHeightLevel3  = -1.5f;
		Platform* platform = 
			new Platform(Vector3(-20.0f, -13.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 
										Rectangle(1000.0f, 1.0f));
		platform->sprite()->addAnimation("basic", g_platformAnimation);
		m_data->objects.push_back(platform);

      
      for(CHUint i=0; i<20; ++i)
      {
			platform = 
			   new Platform(Vector3( float((i + 1) * 40), LedgeHeightLevel2, 0.0f), 
                         Vector3(0.0f, 0.0f, 0.0f), 
							    Rectangle(20.0f, 1.0f));
			platform->sprite()->addAnimation("basic", g_platformAnimation);
	      m_data->objects.push_back(platform);

			platform = 
			   new Platform(Vector3( float((i + 1) * 40 + 10), LedgeHeightLevel3, 0.0f), 
                                 Vector3(0.0f, 0.0f, 0.0f), 
										   Rectangle(5.0f, 1.0f));
			platform->sprite()->addAnimation("basic", g_platformAnimation);
	      m_data->objects.push_back(platform);
      }
	}
}
void
LevelObjects::endLevel()
{	
	m_data->clear();
}

Status 
LevelObjects::load()
{
   OGG_HANDLE handle = OggController::loadOggFile("sound\\SGDieOGG.ogg");
   OggController::getOggPlayer(handle)->setVolume(0.1f);
   ScribbleEnemy::setDeathSound(handle);
   handle = OggController::loadOggFile("sound\\SGHitOGG.ogg");
   OggController::getOggPlayer(handle)->setVolume(0.2f);
   ScribbleEnemy::setHitSound(handle);
   handle = OggController::loadOggFile("sound\\SGSpawnOGG.ogg");
   ScribbleEnemy::setSpawnSound(handle);
	
	// load images
	RES_HANDLE  r17, r18, r19, r20, r21, r22, r23,
               r24, r25, r26, r27, r28, r29, r30,
					r31, r32, r33, r34, r35;
	Status stat;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f1.bmp", r17);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f2.bmp", r18);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f3.bmp", r19);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f4.bmp", r20);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f5.bmp", r21);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f6.bmp", r22);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f7.bmp", r23);		if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f1_mask.bmp", r24); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f2_mask.bmp", r25); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f3_mask.bmp", r26); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f4_mask.bmp", r27); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f5_mask.bmp", r28); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f6_mask.bmp", r29); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/scribbleman_walk_f7_mask.bmp", r30); if (StatusFailed(stat)) return StatusFailure;
	stat = ResourceLibrary::LoadResource("images/platformblock.bmp", r31); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/boss1.bmp", r32); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/boss2.bmp", r33); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/boss1_mask.bmp", r34); if (StatusFailed(stat)) return StatusFailure;
   stat = ResourceLibrary::LoadResource("images/boss2_mask.bmp", r35); if (StatusFailed(stat)) return StatusFailure;

   g_scribbleBossAnimation = new Animation(2);
   g_scribbleBossAnimation->pushFrameAtEnd(Frame(r32,r34));
   g_scribbleBossAnimation->pushFrameAtEnd(Frame(r33,r35));

	
   g_scribbleManAnimation = new Animation(5);
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r17,r24));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r18,r25));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r19,r26));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r20,r27));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r21,r28));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r22,r29));
   g_scribbleManAnimation->pushFrameAtEnd(Frame(r23,r30));	
	
	g_platformAnimation = new Animation(50);
	g_platformAnimation->pushFrameAtEnd(Frame(r31, r31));

	return StatusSuccess;
}

void
LevelObjects::removeStaleObjects()
{
   for( GameObjects::iterator iter = m_data->objects.begin();
        iter != m_data->objects.end();
        ++ iter )
	{
		if((*iter)->isMarkedForRemoval())
		{
         m_data->objects.erase(iter);
         if( m_data->objects.empty() )
            break;
         else
            iter = m_data->objects.begin();
		}
	}
}


void
LevelObjects::sort()
{	
	removeStaleObjects();

   bool exchanges;
   CHUint sz = (CHUint)m_data->objects.size();
   do 
   {
      exchanges = false;
      for( CHUint i=0; i<sz-1; i++) 
      {
         if (m_data->objects[i]->position().x > m_data->objects[i+1]->position().x) 
         {
            GameObject* temp = m_data->objects[i]; 
            m_data->objects[i] = m_data->objects[i+1]; 
            m_data->objects[i+1] = temp;
            exchanges = true; 
         }
      }
   } 
   while (exchanges);
}

Status 
LevelObjects::process()
{
	removeStaleObjects();
	return StatusSuccess;
}

bool
LevelObjects::isObjectWithinPortal(const GameObject& object) const
{
	Vector3 llObject(object.position().x, object.position().y, 0.0f);
	Vector3 urObject(object.position().x + object.polygon().width,
						  object.position().y + object.polygon().height,
						  0.0f);
	Vector3 llPortal(m_portalCenterX - (0.5f * m_portalWidth), 
						  m_portalCenterY - (0.5f * m_portalHeight), 
						  0.0f);
	Vector3 urPortal(m_portalCenterX + (0.5f * m_portalWidth),
						  m_portalCenterY + (0.5f * m_portalHeight),
						  0.0f);
   return utils3D::RectToRectCollision2D(llObject, urObject, llPortal, urPortal);
}

GameObjects
LevelObjects::getObjectsInPortal()
{
   GameObjects objects;

   CHUint sz = (CHUint)m_data->objects.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if( isObjectWithinPortal( *(m_data->objects[i])) )
         objects.push_back(m_data->objects[i]);
   }

	return objects;
}

GameObjects 
LevelObjects::getAllObjects() const
{
	return m_data->objects;
}