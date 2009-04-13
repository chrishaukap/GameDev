#ifndef RESOURCECONTROLLER_H
#define RESOURCECONTROLLER_H

#include "cdhBase.h" 

namespace CDH
{

typedef CHUlong RES_HANDLE;

// HAUKAP - will handle all resources
class ResourceLibrary
{
public:
	static Status initialize();
	static Status terminate();
   
   static RES_HANDLE LoadResource(const char* resource);
   static AUX_RGBImageRec* getRGBImage(RES_HANDLE);  

private:
   ResourceLibrary();
   ResourceLibrary(const ResourceLibrary& C);
   const ResourceLibrary& operator=(const ResourceLibrary& C);
};

}

#endif // RESOURCECONTROLLER_H