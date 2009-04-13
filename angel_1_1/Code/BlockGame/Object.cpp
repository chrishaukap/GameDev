#include "stdafx.h"

#include "Object.hpp"

Object::Object(char c, char *assetDir)
{
	mIcon = c;
	mAsset.mFilename = assetDir;
	mProps = 0x0;	// no properties!
}

Object::~Object()
{
}

void Object::Render()
{

}