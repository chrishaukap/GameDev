#ifndef OGGPLAYER_H
#define OGGPLAYER_H

#include "cdhBase.h" 

namespace CDH
{
class OggPlayer
{
public:
   OggPlayer(const char* oggFile);
   ~OggPlayer();

   Status play();
   Status pause();
   Status stop();
   
   float getVolume() const;
   Status setVolume(float volume);
   Status setLooping(bool looping);

protected:
   Status destroySound();
   Status initSound(const char* oggFile);
private:
   class OggFileData;
   OggFileData* m_ogg;
   class OggLoader;
   OggLoader* m_oggLoader;
};

}

#endif // OGGPLAYER_H