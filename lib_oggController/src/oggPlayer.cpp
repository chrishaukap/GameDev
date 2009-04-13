#include <al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <assert.h>

#include "oggPlayer.h"
#include "thread.h"

namespace CDH
{
   
class OggPlayer::OggFileData 
{
public:
   OggFileData(){}
   ~OggFileData(){}

   vorbis_info* m_pInfo;
   OggVorbis_File m_oggFile;
   ALuint m_bufferID;
   ALuint m_sourceID;
   std::vector<char> m_bufferData;
};

#define BUFFER_SIZE 32768 // 32 KB buffer
static void LoadOGG(const char *fileName, 
                    std::vector<char> &buffer, 
                    OggVorbis_File &oggFile,
                    vorbis_info*& pInfo)
{
   int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
   int bitStream;
   long bytes;
   char array[BUFFER_SIZE]; // Local fixed size array
   FILE *f;

   f = fopen(fileName, "rb");
   assert(f != NULL);
   if(f == NULL)
   {
      // do something
   }

   if (ov_open(f, &oggFile, NULL, 0) != 0)
   {
      // do something
   }

   pInfo = ov_info(&oggFile, -1);

   //format = pInfo->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
   //freq = pInfo->rate;

   do
   {
      bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

      if (bytes < 0)
      {
         //ov_clear(&oggFile);
         // do something
      }

      buffer.insert(buffer.end(), array, array + bytes);
   }
   while (bytes > 0);

   //ov_clear(&oggFile);
}

class OggPlayer::OggLoader : public Thread
{
public:
   OggLoader(const char* filename, 
             OggFileData* oggFileData) :
      m_filename(filename),
      m_oggData(oggFileData)      
   {}
   ~OggLoader() {}

protected:
   Status task()
   {
      alGenBuffers(1, &m_oggData->m_bufferID);
      alGenSources(1, &m_oggData->m_sourceID);

      // Set source and listener to same location
      alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
      alSource3f(m_oggData->m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

      LoadOGG(m_filename, m_oggData->m_bufferData, m_oggData->m_oggFile, m_oggData->m_pInfo);
      
      ALenum format = m_oggData->m_pInfo->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

      // Upload sound data to buffer
      alBufferData(m_oggData->m_bufferID, format, 
                   &m_oggData->m_bufferData[0], 
                   static_cast<ALsizei>(m_oggData->m_bufferData.size()), 
                   m_oggData->m_pInfo->rate);

      // Attach sound buffer to source
      alSourcei(m_oggData->m_sourceID, AL_BUFFER, m_oggData->m_bufferID);

      return StatusSuccess;
   }
private:
   const char* m_filename;
   OggFileData* m_oggData;
};

} // end CDH namespace

using namespace CDH;

OggPlayer::OggPlayer(const char* filename) : 
   m_ogg(new OggFileData),
   m_oggLoader(NULL)
{   
   Status stat = initSound(filename);
}
OggPlayer::~OggPlayer()
{
   Status stat = destroySound();
   delete m_ogg;
   delete m_oggLoader;
}

Status
OggPlayer::destroySound()
{
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
      
   int err = ov_clear(&m_ogg->m_oggFile); // HAUKAP - ret value

   alDeleteBuffers(1, &m_ogg->m_bufferID);
   alDeleteSources(1, &m_ogg->m_sourceID);

   return StatusSuccess;
}

Status
OggPlayer::initSound(const char* filename)
{
   assert(m_ogg != NULL);

   m_oggLoader = new OggLoader(filename, m_ogg);
   Status stat = m_oggLoader->execute();

   return StatusSuccess;
}

Status
OggPlayer::setVolume(float volume)
{
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
   alSourcef(m_ogg->m_sourceID, AL_GAIN, volume);

   return StatusSuccess;
}

Status
OggPlayer::setLooping(bool looping)
{
   assert(m_oggLoader != NULL);
   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());
   assert(m_ogg != NULL);
   alSourcei(m_ogg->m_sourceID, AL_LOOPING, looping ? 1 : 0 );

   return StatusSuccess;
}

float
OggPlayer::getVolume() const
{
   assert( !"not implemented" );
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
   //alSourcef(m_ogg->m_sourceID, AL_GAIN, volume);

   return StatusSuccess;
}

Status 
OggPlayer::play()
{
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
   alSourcePlay(m_ogg->m_sourceID);

   return StatusSuccess;
}
Status 
OggPlayer::pause()
{
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
   alSourcePause(m_ogg->m_sourceID);

   return StatusSuccess;
}
Status 
OggPlayer::stop()
{
   assert(m_oggLoader != NULL);

   while(m_oggLoader->isRunning())
      Sleep(10);

   assert(!m_oggLoader->isRunning());

   assert(m_ogg != NULL);
   alSourceStop(m_ogg->m_sourceID);

   return StatusSuccess;
}

