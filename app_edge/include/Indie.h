#pragma once

#include "cdhBase.h"
#include "Actor.h"
#include "iCollidable.h"

namespace CDH
{
	namespace Edge
	{
		class Indie : public Actor, public iCollidable
		{
		public:
			Indie();
			~Indie();

			void Update(float dt);

         bool isDead() const {return m_state == eDead;}
			void killMe();
         bool isAlive() const {return m_state == eLiving;}
         bool isMarkedForDeletion() const {return m_state == eDeleteMe;}
         void markMeForDeletion() {m_state = eDeleteMe;}

         IMPLEMENT_INDEXABLE_CLASS(Indie)
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