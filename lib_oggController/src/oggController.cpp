#include <AL/alut.h>
#include <assert.h>
#include <map>

#include "oggController.h"
#include "oggPlayer.h"

using namespace CDH;

bool OggController::m_initialized = false;
static std::map<OGG_HANDLE, OggPlayer*> g_oggPlayers;

Status
OggController::oggInitialize()
{
   if(!m_initialized)
   {
      // HAUKAP - return values
      alutInit(NULL, NULL); 
      m_initialized = true;
   }

   return StatusSuccess;
}
Status
OggController::oggTerminate()
{   
   std::map<OGG_HANDLE,OggPlayer*>::iterator iter = g_oggPlayers.begin();
   while( iter != g_oggPlayers.end() )
   {
      OggPlayer *player = iter->second;
      delete player;
      ++iter;
   }
   g_oggPlayers.clear();
   
   // HAUKAP - return values
   alutExit();
   return StatusSuccess;
}

OGG_HANDLE
OggController::loadOggFile(const char* oggFile)
{
   assert(m_initialized);
   assert(oggFile != NULL && *oggFile != NULL);

   OggPlayer* ret = new OggPlayer(oggFile);
   OGG_HANDLE handle = (OGG_HANDLE) &(*ret);
   g_oggPlayers[handle] = ret;
   return handle;
}

OggPlayer* 
OggController::getOggPlayer(OGG_HANDLE handle)
{
   return g_oggPlayers[handle];
}
