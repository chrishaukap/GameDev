#pragma once

#include "cdhBase.h"
#include "Actor.h"

namespace CDH
{
	namespace Edge
	{
		class TimHead : public Actor
		{
		public:
			TimHead();
			~TimHead();

			void Update(float dt);

		private:
		};
	}
}