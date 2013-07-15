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
			void demote() {m_leader = false;}
			bool isLeader() {return m_leader;}
			void joinIndie(Indie& indie);

			bool isOnTeam() const {return m_teamNumber != -1;}
			int getTeamNumber() const { return m_teamNumber; }
			void assignToNextTeam() {m_teamNumber = getNextTeamNumber();}
			void assignToNextTeam(const Indie&);
			void setTeamColor();

         IMPLEMENT_INDEXABLE_CLASS(Indie)
		private:
			static int getNextTeamNumber() {return ++NextTeamNumber;}
			bool onTheSameTeam(const Indie& indie);
			void assignAdjacentsToTeam(const Indie& indie);
			void setTeamColorRec(float r, float g, float b);
			void move(float dt);

			typedef std::vector<Indie*> INDIE_LIST;
			void moveRec(INDIE_LIST& visited, float dX, float dY);

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
			INDIE_LIST m_adjacentIndies; 
			bool m_leader;
			int m_teamNumber;
			static int NextTeamNumber;
		};
	}
}