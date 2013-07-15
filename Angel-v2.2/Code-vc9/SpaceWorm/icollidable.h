#pragma once

#include "cdhBase.h"
#include "indexableClass.h"

class iCollidable
{
public:
   iCollidable(){}
   virtual ~iCollidable() {}
   IMPLEMENT_INDEXABLE_CLASS(iCollidable)
private:
};