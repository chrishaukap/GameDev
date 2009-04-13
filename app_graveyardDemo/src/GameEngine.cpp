#include "GameEngine.h"

#include "assert.h"
#include "tree/Tree.h"
#include "tree/branch.h"
#include "tree/limb.h"
#include "peeps/peep.h"
#include "peeps/peepDriver.h"
#include "fence.h"
#include "camera.h"
#include <vector>
#include <float.h>

#include "vector3D.h"
#include "defines.h"
#include "graveyard.h"
#include "blood/bloodEngine.h"
#include "blood/bloodSpout.h"

#include "oggController.h"
#include "oggPlayer.h"

using namespace CDH;

GameEngine::GameEngine() :
   m_tree(NULL),
   m_fence(),
   m_camera(NULL),
   m_bloodEngine(new BloodEngine()),
   m_peepDriver(new PeepDriver(*m_bloodEngine))
{   
}

GameEngine::~GameEngine()
{
   destroy();
}

static OGG_HANDLE g_peepDeathSoundHandle = 0;
static OGG_HANDLE g_backgroundMusicHandle = 0;
void GameEngine::destroy()
{
   OggController::oggTerminate();
   delete m_tree;
   delete m_camera;
   delete m_peepDriver;
   delete m_bloodEngine;
   destroyFence();
}

const std::vector<FenceSegment*>::const_iterator 
GameEngine::getFenceSegments() const
{
   return m_fence.begin();
}

void GameEngine::initializePreCredits()
{
   Status stat = OggController::oggInitialize();   
   assert(stat == StatusSuccess);

   if(PLAY_SOUND)
   {
      g_peepDeathSoundHandle = OggController::loadOggFile( "snd\\death1.ogg" );
      g_backgroundMusicHandle = OggController::loadOggFile( "snd\\mutilate.ogg" );
   }

   m_tree = new Tree(Vector3(0.0f, 0.0f, -10.0f));
   m_camera = new Camera;
   populateFence();
   m_peepDriver->initPeeps();
   initTree();
}
void GameEngine::initializePostCredits()
{
   if(PLAY_SOUND)
   {
      OggController::getOggPlayer(g_peepDeathSoundHandle)->setVolume(1.5f);
      OggController::getOggPlayer(g_backgroundMusicHandle)->play();
   }
}


void GameEngine::populateFence()
{
   for(float x = GraveyardBounds::farLeft.x;
       x <= GraveyardBounds::farRight.x;
       x += 0.5f)
   {
      m_fence.push_back(
         new FenceSegment( Vector3(x, 0.0f, GraveyardBounds::backWallZ), 
                           0, 1.0f ));
      m_fence.push_back(
         new FenceSegment( Vector3(x, 0.0f, GraveyardBounds::frontWallZ), 
                           0, 1.0f ));
   }
   for(float z = GraveyardBounds::farLeft.z;
       z <= GraveyardBounds::nearLeft.z;
       z += 0.5f)
   {
      m_fence.push_back(
         new FenceSegment( Vector3(GraveyardBounds::leftWallX, 0.0f, z), 
                           0, 1.0f ));
      m_fence.push_back(
         new FenceSegment( Vector3(GraveyardBounds::rightWallX, 0.0f, z), 
                           0, 1.0f ));
   }
}

// HAUKAP - should be moved to fence
void GameEngine::destroyFence()
{
   CHUint size = (CHUint)m_fence.size();
   for(CHUint i=0; i<size; ++i)
      delete m_fence[i];
   m_fence.clear();
}

void GameEngine::render() const
{
   m_tree->render();
   m_peepDriver->renderPeeps();
   m_peepDriver->renderParts();
   m_bloodEngine->render();
}

void GameEngine::assignPeepsForPursuit()
{
   while(m_tree->canPursue())
   {
      PeepDriver::PEEPS::iterator curPeep = m_peepDriver->getPeeps().begin();   
      float shortestDistance = FLT_MAX;
      Peep* thePeepToPursue = NULL;

      while(curPeep != m_peepDriver->getPeeps().end())
      {
         if((*curPeep)->isFree())
         {
            float dist = utils3D::distanceP1P2( (*curPeep)->position(), m_tree->position() );       
            if(dist < shortestDistance)
            {
               shortestDistance = dist;
               thePeepToPursue = (*curPeep);
            }
         }
         ++curPeep;
      }  

      if( thePeepToPursue )
      {
         m_tree->pursue( *thePeepToPursue );
         shortestDistance = FLT_MAX;
         thePeepToPursue = NULL;
      }
      else
         break;

   }
}

void GameEngine::initTree()
{  
}

void GameEngine::processTree()
{
   assert(m_tree != NULL);   

   assignPeepsForPursuit();
   bool targetAquired = false;
   m_tree->process(targetAquired);
   if( targetAquired && g_peepDeathSoundHandle )
      OggController::getOggPlayer(g_peepDeathSoundHandle)->play();
}

void GameEngine::processPeeps()
{   
   m_peepDriver->process();
}

void GameEngine::processFence()
{

}
void GameEngine::processCamera()
{
   assert(m_camera != NULL);
   m_camera->process();
}

void GameEngine::process()
{   
   processCamera();
#ifndef TEST_CODE
   processTree();
   processBloodEngine();
   processPeeps();  
   processFence();   
#endif
}

void GameEngine::processBloodEngine()
{
   m_bloodEngine->process();
}

CHUint GameEngine::numSpouts() const
{
   return m_bloodEngine->numSpouts();
}
CHUint GameEngine::numDroplets() const
{
   return m_bloodEngine->numDroplets();
}

CHUint GameEngine::numPeepParts() const
{
   return m_peepDriver->numPeepParts();
}