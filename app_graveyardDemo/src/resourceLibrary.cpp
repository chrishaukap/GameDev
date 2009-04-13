#include <assert.h>
#include <map>
#include <gl/glaux.h>		// Header File For The Glaux Library

#include "resourceLibrary.h"

using namespace CDH;

// HAUKAP - 
static std::map<RES_HANDLE, AUX_RGBImageRec*> g_rgbImageMap;
 
Status
ResourceLibrary::initialize()
{
   return StatusSuccess;
}
Status
ResourceLibrary::terminate()
{   
   std::map<RES_HANDLE, AUX_RGBImageRec*>::iterator iter = g_rgbImageMap.begin();
   while( iter != g_rgbImageMap.end() )
   {
      AUX_RGBImageRec *res = iter->second;
      delete res;
      ++iter;
   }
   g_rgbImageMap.clear();
   
   return StatusSuccess;
}

static 
AUX_RGBImageRec* LoadBMP(const char *filename)                
{
	AUX_RGBImageRec* ret = NULL;
	assert( filename && *filename );
	if(filename != NULL)
	{
		FILE *file = fopen( filename, "r" );  
		if (file)                        
		{
			fclose(file);                
			ret = auxDIBImageLoad(filename);    
		}
		else
		{
			std::string msg = "Could not find the file: ";
			msg += filename;
			// HAUKAP - unix/windos messaging/error handling system
			MessageBox( NULL, msg.c_str(), "Required file missing", MB_OK );
		}	
	}
	return ret; 
}

RES_HANDLE
ResourceLibrary::LoadResource(const char* bmp)
{	  
	RES_HANDLE ret = 0;

   assert(bmp != NULL && *bmp != NULL);
	if(bmp && *bmp)
	{    
      AUX_RGBImageRec* resource = LoadBMP(bmp);
		assert(resource != NULL);
		if(resource)
		{
			ret = (RES_HANDLE) &(*resource); // HAUKAP - *__w64 pointer truncation warning
			g_rgbImageMap[ret] = resource;
		}
	}
	return ret;		
}

AUX_RGBImageRec* 
ResourceLibrary::getRGBImage(RES_HANDLE handle)
{
	// HAUKAP  - assuming handle is an rgbimage
   return g_rgbImageMap[handle];
}
