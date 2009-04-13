#include <assert.h>
#include <gl/glaux.h>
#include <stdio.h>

#include "resource/bitmapImage.h"

using namespace CDH;

// opens the bitmap filename.  
// caller responsible for deleting file
// returns StatusFileNotFound if filename is not found
static Status 
LoadBMP(const char *filename, AUX_RGBImageRec*& file)                
{
	Status stat = StatusSuccess;
	file = NULL;

	assert( filename && *filename );
	if(filename && *filename)
	{
		FILE *tmp = fopen( filename, "r" );  
		if (tmp)                        
		{
			fclose(tmp);                
			file = auxDIBImageLoad(filename);    
		}
		else
		{
			stat = StatusFileNotFound;
		}		
	}
	return stat; 
}

struct BitmapImage::Data
{
	Data() {}
	~Data() {delete data;} // HAUKAP - need to delete?
	AUX_RGBImageRec* data;
};

BitmapImage::BitmapImage() : 
	ImageBase(),
	m_data(NULL)
{}
BitmapImage::~BitmapImage()
{
	delete m_data;
}

Status 
BitmapImage::initialize(const char* filename)
{
	assert(filename && *filename);
	Status stat = StatusInvalidArgs;
	if(filename && *filename)
	{
		m_data = new Data;
		stat = LoadBMP(filename, m_data->data);
	}
	return stat;
}

unsigned char* 
BitmapImage::data() const
{
	assert(m_data != NULL);
	return m_data->data->data;
}
CHUint 
BitmapImage::width() const
{
	assert(m_data != NULL);
	return m_data->data->sizeX;
}
CHUint
BitmapImage::height() const
{
	assert(m_data != NULL);
	return m_data->data->sizeY;
}