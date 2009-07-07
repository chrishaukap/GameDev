#pragma once

#include "cdhBase.h"
#include <vector>

struct Vector2;
namespace CDH
{
	namespace Edge
	{
		class Indie;
      class Tim;
		class Game
		{
		public:
			Game(CHUint rows, CHUint cols);
			~Game();

         void update(float dt);
         void cleanup();

			void start();
			void stop();

         void moveTimTo(const Vector2& p);
		private:
			std::vector<Indie*> m_indies;
         Tim* m_tim;
		};
	}
}