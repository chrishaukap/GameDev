#pragma once

#include "cdhBase.h"
#include "indexableClass.h"

namespace CDH
{
	namespace Cole
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
}