#pragma once

#include "cdhBase.h"
#include "Actor.h"

namespace CDH
{
	namespace Edge
	{
		class Indie : public Actor
		{
		public:
			Indie();
			~Indie();

			void Update(float dt);

		private:
		};
	}
}