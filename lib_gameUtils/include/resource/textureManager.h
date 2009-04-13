#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "cdhBase.h"
namespace CDH
{

class TextureManager
{
public:
   static Status initialize();
   static bool isInitialized() {return m_initted;}
   static Status terminate();

   static CHUint getTextureHandle(RES_HANDLE resHandle);
private:   
   static bool m_initted;

   TextureManager();
   TextureManager(const TextureManager& C);
   const TextureManager& operator=(const TextureManager& C);
};
}

#endif //TEXTUREMANAGER_H
