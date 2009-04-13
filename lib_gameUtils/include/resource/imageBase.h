#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include "cdhBase.h" 

namespace CDH
{

class ImageBase
{
public:
   ImageBase();
   virtual ~ImageBase();

	virtual Status initialize(const char* filename) = 0;

	virtual unsigned char* data() const = 0;
	virtual CHUint width() const = 0;
	virtual CHUint height() const = 0;
   
private:
   ImageBase(const ImageBase& C);
   const ImageBase& operator=(const ImageBase& C);
};

}

#endif // BITMAPIMAGE_H