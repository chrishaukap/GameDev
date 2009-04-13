#include <assert.h>
#include <map>

#include "cdhBase.h"

#include "resource/resourceLibrary.h"
#include "resource/bitmapImage.h"

using namespace CDH;

typedef std::map<RES_HANDLE, ImageBase*> ImageHandleMap;
static ImageHandleMap g_imageHandleMap;
static ImageHandleMap::iterator g_imageHandleMapIter;
bool ResourceLibrary::m_initted = false;
 
Status
ResourceLibrary::initialize()
{
   if(!m_initted)
   {
      g_imageHandleMapIter = g_imageHandleMap.begin();
      m_initted = true;
   }
   return StatusSuccess;
}
Status
ResourceLibrary::terminate()
{   
   g_imageHandleMapIter = g_imageHandleMap.begin();
   while( g_imageHandleMapIter != g_imageHandleMap.end() )
   {
      ImageBase *res = g_imageHandleMapIter->second;
      delete res;
      ++g_imageHandleMapIter;
   }
   g_imageHandleMap.clear();
   
   return StatusSuccess;
}

Status 
ResourceLibrary::LoadResource(const char* filename, RES_HANDLE& handle)
{	 
	Status stat = StatusInvalidArgs;
	handle = 0;

   assert(filename != NULL && *filename != NULL);
	if(filename && *filename)
	{    
		// HAUKAP - we can figure out different image types here
      ImageBase* img = new BitmapImage;
		stat = img->initialize(filename);

		handle = (RES_HANDLE) &(*img); // HAUKAP - *__w64 pointer truncation warning
		g_imageHandleMap[handle] = img;
	}
	return stat;		
}

const ImageBase* 
ResourceLibrary::getImage(RES_HANDLE handle)
{
	const ImageBase* img = g_imageHandleMap[handle];
	assert(img != NULL);
   return img;
}

CHUint 
ResourceLibrary::getNumResources()
{
   return (CHUint)g_imageHandleMap.size();
}

RES_HANDLE 
ResourceLibrary::getFirstResource()
{
   if(g_imageHandleMap.empty())
      return InvalidHandle;

   g_imageHandleMapIter = g_imageHandleMap.begin();
   return g_imageHandleMapIter->first;
}

RES_HANDLE 
ResourceLibrary::getNextResource()
{
   ++g_imageHandleMapIter;

   if(g_imageHandleMapIter == g_imageHandleMap.end())
      return InvalidHandle;

   return g_imageHandleMapIter->first;
}
