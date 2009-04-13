#include "oggController.h"
#include "oggPlayer.h"

#include <windows.h>
#include <assert.h>

using namespace CDH;

int main()
{
   Status stat = OggController::oggInitialize();
   assert(stat == StatusSuccess);

   OGG_HANDLE h1 = OggController::loadOggFile("mutilate.ogg");
   OGG_HANDLE h2 = OggController::loadOggFile("Bomb.ogg");
   OggPlayer* p1 = OggController::getOggPlayer(h1);
   OggPlayer* p2 = OggController::getOggPlayer(h2);

   assert(p1 != NULL &&
          p2 != NULL );

   stat = p1->play(); assert(stat == StatusSuccess);
   Sleep(3000);
   p1->setVolume(0.75f);
   Sleep(3000);
   p1->setVolume(0.5f);
   Sleep(3000);
   p1->setVolume(0.25f);
   Sleep(3000);
   p1->setVolume(0.0f);
   Sleep(3000);
   p1->setVolume(0.75f);
   Sleep(1500);
   p1->pause();
   Sleep(1500);
   p1->play();
   Sleep(1500);

   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(10);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(20);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(40);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(100);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(200);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(400);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(1000);
   stat = p2->play(); assert(stat == StatusSuccess);
   Sleep(2000);
   stat = p2->play(); assert(stat == StatusSuccess);

   stat = OggController::oggTerminate();
   assert(stat == StatusSuccess);

   return 0;
}


