#pragma once

#include "cdhBase.h"
#include "threadPool.h"
#include "TTTPlayerMove.h"
#include "TTTAIUtils.h"
#include "TTTGameState.h"

namespace TicTacToe
{
   struct TaskObject
   {      
      // takes ownership of grid
      TaskObject(TTTGrid* grid, NodeState& originalMove, TTTPlayer whosTurn) : 
         m_grid(grid),
         m_theMove(originalMove),
         m_turn(whosTurn)
      {}
      ~TaskObject();

      TTTGrid* m_grid;
      NodeState& m_theMove;
      TTTPlayer m_turn;
   private:
      TaskObject();
   };

   class mytimer;
   class TTTGameState;
   class ThinkTask : public CDH::ThreadTask
	{
	public:
      // takes ownership of taskObject
      ThinkTask(TaskObject* taskObject,
                unsigned int depth);
		~ThinkTask();
      Status doTask();

	private:
		TaskObject* m_taskObject;
		unsigned int m_depth;
      mytimer* m_mytimer;
      
		ThinkTask(const ThinkTask& T);
		const ThinkTask& operator=(const ThinkTask& T);
	};
}