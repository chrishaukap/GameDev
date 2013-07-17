#pragma once

#include "TTTPlayerMove.h"
#include <assert.h>
#include <map>
#include <vector>

namespace CDH
{
   class ThreadPool;
}

namespace TicTacToe
{	
	enum Directions
	{
		Up =0, Down, Left, Right, UpperRight, UpperLeft, LowerRight, LowerLeft
	};

   struct NodeState
   {
      NodeState(): m_value(0), m_move() {}
		NodeState(int value, TTTPlayerMove move) : m_value(value), m_move(move) {}
      int m_value;
      TTTPlayerMove m_move;    
   };

   class TTTGrid;
	typedef std::vector<NodeState> AIMOVES;
	class TTTGameState;
	class TTTAIUtils 
	{
	public:		
      static void init();
      static void uninit();
      static TTTPlayerMove getBestMove(TTTGameState& gameState, unsigned int depth);
      static TTTPlayerMove getBestMoveMultiThreaded(TTTGameState& gameState, unsigned int depth);
		
      static NodeState getMinMove(const TTTGrid& grid, const TTTPlayerMove& previousMove, TTTPlayer turn, const NodeState& nodeToBeat, unsigned int depth, bool root);
	private:
		TTTAIUtils();

      static const unsigned int m_maxThreads;
      static CDH::ThreadPool *m_threadPool;

		static unsigned int NumCellsInDirection( const TTTGrid& grid, 
															  unsigned int j, unsigned int i, 
															  Directions direction,
															  TTTPlayer playerOfInterest);
		static NodeState getMaxMove(const TTTGrid& grid, const TTTPlayerMove& previousMove, TTTPlayer turn, const NodeState& nodeToBeat, unsigned int depth, bool root);
      static NodeState getMaxMoveMultiThreaded(const TTTGrid& grid, const TTTPlayerMove& previousMove, TTTPlayer turn, const NodeState& nodeToBeat, unsigned int depth);

		static unsigned int AssessValue(const TTTGrid& grid, unsigned int j, unsigned int i, TTTPlayer playerOfInterest);
      static AIMOVES getPossibleMoves(const TTTGrid& grid, TTTPlayer turn);
      static AIMOVES getPossibleMovesWithValues(const TTTGrid& grid, TTTPlayer turn);
      static bool hasNonEmptyNeighbor(const TTTGrid& grid, unsigned int j, unsigned int i);
	};
}