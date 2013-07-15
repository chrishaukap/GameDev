#pragma once

#include "TicTacToe.h"

namespace TicTacToe
{
   class MiniMaxTree
   {
   public:
	   // depth is number of AI moves in the future to process
	   static TTTPlayerMove getBestMove(TTTGameState& gameState, unsigned int depth);
   private:      
   };

}