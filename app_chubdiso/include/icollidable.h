#pragma once

#include "cdhBase.h"
#include "indexableClass.h"

namespace CDH
{
	namespace Chubdiso
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