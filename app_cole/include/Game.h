#pragma once

#include "cdhBase.h"
#include <vector>

struct Vector2;
class PhysicsActor;
namespace CDH
{
	namespace Cole
	{
		class Game
		{
		public:
			Game(CHUint rows, CHUint cols);
			~Game();
			
         void update(float dt);
         void cleanup();

			void start();
			void stop();

			// debug draw
			const class ColeAvatar* cole() const {return m_cole;}

		private:
         ColeAvatar* m_cole;
			PhysicsActor* m_ground;
		};
	}
}