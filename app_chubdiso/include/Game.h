#pragma once

#include "cdhBase.h"
#include <vector>

struct Vector2;
class PhysicsActor;
namespace CDH
{
	namespace Chubdiso
	{
		class Game
		{
		public:
			Game(CHUint rows, CHUint cols);
			~Game();
			
			typedef std::vector<class Indie*> INDIE_LIST;
			typedef std::vector<class Money*> MONEY_LIST;

         void update(float dt);
         void cleanup();

			void start();
			void stop();
		private:
         class Jake* m_jake;
		};
	}
}