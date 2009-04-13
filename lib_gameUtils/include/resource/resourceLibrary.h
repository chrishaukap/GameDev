#ifndef RESOURCECONTROLLER_H
#define RESOURCECONTROLLER_H

#include "cdhBase.h" 

namespace CDH
{

class ImageBase;
class ResourceLibrary
{
public:
	static Status initialize();
   static bool isInitialized() {return m_initted;}
	static Status terminate();
   
   static Status LoadResource(const char* filename, RES_HANDLE& handle);
   static const ImageBase* getImage(RES_HANDLE);

   static CHUint getNumResources();
   static RES_HANDLE getFirstResource();
   
   // returns next resource handle, or InvalidHandle if noneleft
   static RES_HANDLE getNextResource();   

private:
   static bool m_initted;

   ResourceLibrary();
   ResourceLibrary(const ResourceLibrary& C);
   const ResourceLibrary& operator=(const ResourceLibrary& C);
};

}

#endif // RESOURCECONTROLLER_H