#pragma once

#include "cdhBase.h"
#include <vector>

namespace CDH
{
	namespace Edge
	{
		class Indie;
		class Game
		{
		public:
			Game(CHUint rows, CHUint cols);
			~Game();

         void update(float dt);
			void start();
			void stop();
		private:
			std::vector<Indie*> m_indie;
		};
	}
}