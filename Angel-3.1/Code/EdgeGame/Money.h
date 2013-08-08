#pragma once

#include "Actors/Actor.h"
#include "iCollidable.h"

namespace Edge
{
	class Money : public Actor, public iCollidable
	{
	public:
		Money();
		~Money();
			
		bool isMarkedForDeletion() const {return m_state == eDeleteMe;}
      void markMeForDeletion() {m_state = eDeleteMe;}

      IMPLEMENT_INDEXABLE_CLASS(Money)
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