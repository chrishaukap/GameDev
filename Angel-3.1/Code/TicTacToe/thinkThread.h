#pragma once

#include "thread.h"
#include "TTTPlayerMove.h"
#include "TTTAIUtils.h"


namespace TicTacToe
{
	class TTTGameState;
	class ThinkThread : public CDH::Thread
	{
	public:
		ThinkThread();
		~ThinkThread();

		void stop();
      void think();
		bool isThinking() const;
      bool isReady() const;
		bool isIdle() const;
		void preExecute(const TTTGameState& gameState, int theirBest, unsigned int depth);
		//  HAUKAP - ???? TTTMovesList::TTTMoveValuePair getMove() const {return m_theMove;}

	private:
		enum ThinkThreadState
		{
			ReadyToExecute=0,Idle,Running
		};	
		ThinkThreadState m_state;
		Status task();

		TTTGameState* m_gameState;
		int m_theirBest;
		unsigned int m_depth;
//		TTTMovesList::TTTMoveValuePair m_theMove;
	};
}