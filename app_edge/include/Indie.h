#pragma once

#include "cdhBase.h"
#include "Actor.h"

namespace CDH
{
	namespace Edge
	{
		class Tim;
		class Indie : public Actor
		{
		public:
			Indie();
			~Indie();

			// HAUKAP - cheesey collision response mechanism 
			void Collided(const Tim*);
			void Collided(const Indie*);
			void Update(float dt);

         bool isDead() const {return m_state == eDead;}
         bool isAlive() const {return m_state == eLiving;}
         bool isMarkedForDeletion() const {return m_state == eDeleteMe;}
         void markMeForDeletion() {m_state = eDeleteMe;}

		private:

			enum IndieState
			{
				eUninitialized = -1,
				eLiving = 0,
				eDying,
				eDead,
            eDeleteMe,
				eNumStates
			};
			IndieState m_state;
         CHUint m_dyingCounter;
			std::vector<Indie*> m_adjacentIndies; 
		};
	}
}