#pragma once

#include "indexableClass.h"

namespace Edge
{
	class iCollidable
	{
	public:
      iCollidable(){}
      virtual ~iCollidable() {}
      IMPLEMENT_INDEXABLE_CLASS(iCollidable)
   private:
	};
}