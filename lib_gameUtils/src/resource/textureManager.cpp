#include "resource/textureManager.h"

#include "cdhBase.h"
#include "resource/resourceLibrary.h"
#include "resource/imageBase.h"

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <gl/glaux.h>

#include <map>
#include <assert.h>

using namespace CDH;
typedef std::map<RES_HANDLE, CHUint> ResourceToTextureMap;
static ResourceToTextureMap g_resTextureMap;
static CHUint *g_textures = NULL;
bool TextureManager::m_initted = false;

Status
TextureManager::terminate()
{   
   g_resTextureMap.clear();

   delete [] g_textures;
   g_textures = NULL;
   
   return StatusSuccess;
}

CHUint
TextureManager::getTextureHandle(RES_HANDLE resHandle)
{
   return g_resTextureMap[resHandle];
}

Status 
TextureManager::initialize()
{
   assert(ResourceLibrary::isInitialized());

   CHUint numTextures = ResourceLibrary::getNumResources();
   g_textures = new CHUint[numTextures];

   glGenTextures(numTextures, g_textures); 

   RES_HANDLE handle = ResourceLibrary::getFirstResource();

   for( CHUint i=0; 
        i < numTextures && handle != InvalidHandle; 
        ++i )
   {
      const ImageBase* img = ResourceLibrary::getImage(handle);
      assert(img != NULL);

      glBindTexture(GL_TEXTURE_2D, g_textures[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,  
         img->width(), img->height(), 
         GL_RGB, GL_UNSIGNED_BYTE, 
         img->data());

      // associate the opengl texture handle with our resource handle
      g_resTextureMap[handle] = g_textures[i];

      handle = ResourceLibrary::getNextResource();
   }

   return StatusSuccess;
}



