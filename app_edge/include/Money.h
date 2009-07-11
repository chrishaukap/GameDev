#pragma once

#include "cdhBase.h"
#include "Actor.h"

namespace CDH
{
	namespace Edge
	{
		class Tim;
		class Indie;
		class Money : public Actor
		{
		public:
			Money();
			~Money();

			// HAUKAP - cheesey collision response mechanism 
			void Collided(const Tim*);
			void Collided(const Indie*); 
			
			bool isMarkedForDeletion() const {return m_state == eDeleteMe;}
         void markMeForDeletion() {m_state = eDeleteMe;}

		private:
			
			enum MoneyState
			{
				eExists = -1,
            eDeleteMe,
				eNumStates
			};
			MoneyState m_state;
		};
	}
}