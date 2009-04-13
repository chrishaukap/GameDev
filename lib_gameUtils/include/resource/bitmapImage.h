#ifndef BITMAPIMAGE_H
#define BITMAPIMAGE_H

#include "cdhBase.h" 
#include "imageBase.h"

namespace CDH
{

class BitmapImage :  public ImageBase
{
public:
   BitmapImage();
   virtual ~BitmapImage();

	Status initialize(const char* filename);

	unsigned char* data() const;
	CHUint width() const;
	CHUint height() const;
   
private:
	struct Data;
	Data* m_data;

   BitmapImage(const BitmapImage& C);
   const BitmapImage& operator=(const BitmapImage& C);
};

}

#endif // BITMAPIMAGE_H