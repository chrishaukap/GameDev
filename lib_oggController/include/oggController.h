#ifndef OGGCONTROLLER_H
#define OGGCONTROLLER_H

#include "cdhBase.h" 


namespace CDH
{

typedef CHUlong OGG_HANDLE;

class OggPlayer;
class OggController
{
public:
   static Status oggInitialize();
   static Status oggTerminate();
   
   static OGG_HANDLE loadOggFile(const char* oggFile);
   static OggPlayer* getOggPlayer(OGG_HANDLE);  

private:
   OggController();
   OggController(const OggController& C);
   const OggController& operator=(const OggController& C);

   static bool m_initialized;
};

}

#endif // OGGCONTROLLER_H