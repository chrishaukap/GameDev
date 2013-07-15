#include "stdafx.h"
#include "cdhBase.h"
#include "TTTAIUtils.h"
#include "TicTacToe.h"
#include "TTTGrid.h"
#include "TTTGameState.h"
#include "threadPool.h"
#include "thinkTask.h"

using namespace CDH;
using namespace TicTacToe;

const unsigned int TTTAIUtils::m_maxThreads = 1;
ThreadPool *TTTAIUtils::m_threadPool = NULL;

void TTTAIUtils::init()
{
   if(m_threadPool == NULL )
   {
      m_threadPool = new ThreadPool(m_maxThreads);
      m_threadPool->run();
   }
}
void TTTAIUtils::uninit()
{
   m_threadPool->killAllThreads();
   delete m_threadPool; m_threadPool = NULL;
}

unsigned int 
TTTAIUtils::NumCellsInDirection(const TTTGrid& grid, 
                                CHUint j, CHUint i, 
										  Directions direction,
										  TTTPlayer playerOfInterest)
{
	int idxJ = (int)j, idxI = (int)i;
	switch(direction)
	{
	case Up: ++idxJ; break;
	case Down: --idxJ; break;
	case Left: --idxI; break;
	case Right: ++idxI; break;
	case UpperRight: ++idxJ; ++idxI; break;
	case UpperLeft: ++idxJ; --idxI; break;
	case LowerRight: --idxJ; ++idxI; break;
	case LowerLeft: --idxJ; --idxI; break;
	default: assert(0);
	}
	if(idxJ < 0 || idxJ >= (int)grid.size() || idxI < 0 || idxI >= (int)grid.size())
		return 0;
	else if( grid.grid(idxJ,idxI) != playerOfInterest)
		return 0;
	else
		return 1 + NumCellsInDirection(grid, idxJ,idxI, direction, playerOfInterest);
}
unsigned int 
TTTAIUtils::AssessValue(const TTTGrid& grid,
								CHUint j, CHUint i,
								TTTPlayer playerOfInterest)
{		
	TTTPlayer us = playerOfInterest;
	TTTPlayer them = TTTGameState::getOtherPlayer(playerOfInterest); // HAUKAP - this should be in a utils class

	CHUint numHorizontalUs =   NumCellsInDirection(grid,j,i, Left, us) + NumCellsInDirection(grid,j,i, Right, us);
	CHUint numVerticalUs =     NumCellsInDirection(grid,j,i, Up, us) + NumCellsInDirection(grid,j,i, Down, us);
	CHUint diagonal1Us =       NumCellsInDirection(grid,j,i, UpperRight, us) + NumCellsInDirection(grid,j,i, LowerLeft, us);
	CHUint diagonal2Us =       NumCellsInDirection(grid,j,i, UpperLeft, us) + NumCellsInDirection(grid,j,i, LowerRight, us);
	CHUint numHorizontalThem = NumCellsInDirection(grid,j,i, Left, them) + NumCellsInDirection(grid,j,i, Right, them);
	CHUint numVerticalThem =   NumCellsInDirection(grid,j,i, Up, them) + NumCellsInDirection(grid,j,i, Down, them);
	CHUint diagonal1Them =     NumCellsInDirection(grid,j,i, UpperRight, them) + NumCellsInDirection(grid,j,i, LowerLeft, them);
	CHUint diagonal2Them =     NumCellsInDirection(grid,j,i, UpperLeft, them) + NumCellsInDirection(grid,j,i, LowerRight, them);

	CHUint value = 0;
	if(numHorizontalUs >=4 || numVerticalUs >=4 || diagonal1Us >=4 || diagonal2Us >=4 )
		return INT_MAX;
	else if(numHorizontalThem >=4 || numVerticalThem >=4 || diagonal1Them >=4 || diagonal2Them >=4 )
		value = 3000;
	else if(numHorizontalUs >=3 || numVerticalUs >=3 || diagonal1Us >=3 || diagonal2Us >=3 )
		value = 2500;
	else if(numHorizontalThem >=3 || numVerticalThem >=3 || diagonal1Them >=3 || diagonal2Them >=3 )
		value = 2000;
	else if(numHorizontalUs >=2 || numVerticalUs >=2 || diagonal1Us >=2 || diagonal2Us >=2 )
		value = 1500;
	else if(numHorizontalThem >=2 || numVerticalThem >=2 || diagonal1Them >=2 || diagonal2Them >=2 )
		value = 1000;
	else if(numHorizontalUs >=1 || numVerticalUs >=1 || diagonal1Us >=1 || diagonal2Us >=1 )
		value = 500;
	else if(numHorizontalThem >=1 || numVerticalThem >=1 || diagonal1Them >=1 || diagonal2Them >=1 )
		value = 1;

	value += (numHorizontalUs + numVerticalUs + diagonal1Us + diagonal2Us +
				 numHorizontalThem + numVerticalThem + diagonal1Them + diagonal2Them);
	return value;
}

bool 
TTTAIUtils::hasNonEmptyNeighbor(const TTTGrid& grid, CHUint j, CHUint i)
{
   bool hasNeighbor = false;
   int hiJ = j+1;
   int hiI = i+1;
   int loJ = (int)j-1;
   int loI = (int)i-1;
   int sz = grid.size()-1;
   if( (hiJ <= sz && hiI <= sz &&   grid.grid(hiJ,hiI) != Empty) || 
       (hiJ <= sz &&                grid.grid(hiJ,i)   != Empty) ||
       (hiJ <= sz && loI >= 0 &&    grid.grid(hiJ,loI) != Empty) ||
       (hiI <= sz &&                grid.grid(j  ,hiI) != Empty) ||
       (loI >= 0 &&                 grid.grid(j  ,loI) != Empty) ||
       (loJ >= 0 && hiI <= sz &&    grid.grid(loJ,hiI) != Empty) ||
       (loJ >= 0 &&                 grid.grid(loJ,i)   != Empty) ||
       (loJ >= 0 && loI >= 0 &&     grid.grid(loJ,loI) != Empty) )
       return true;
   return false;
}

AIMOVES
TTTAIUtils::getPossibleMoves(const TTTGrid& grid, TTTPlayer turn)
{
	AIMOVES moves;
   bool allEmpty = true;
   for(CHUint i=0; i< grid.size(); ++i)
	   for(CHUint j=0; j< grid.size(); ++j)
		   if( grid.isCellEmpty(j,i) )
         {
            if(hasNonEmptyNeighbor(grid,j,i))
					moves.push_back(NodeState(0,TTTPlayerMove(j,i,turn)));
         }
         else
            allEmpty = false;

   if(allEmpty)
   {
      // first move... just go in the center
      moves.clear();
      CHUint idx = CHUint(0.5f * grid.size()); 
		moves.push_back(NodeState(0,TTTPlayerMove(idx,idx,turn)));
   }

   return moves;
}

AIMOVES
TTTAIUtils::getPossibleMovesWithValues(const TTTGrid& grid, TTTPlayer turn)
{
	AIMOVES moves;
   for(CHUint i=0; i< grid.size(); ++i)
	   for(CHUint j=0; j< grid.size(); ++j)
		   if( grid.isCellEmpty(j,i) )
         {
            int value = AssessValue(grid, j,i, turn);
            if(value >= 1)
               moves.push_back(NodeState(value,TTTPlayerMove(j,i,turn)));
         }

   assert( !moves.empty());
   return moves;
}


static void 
makeMove( TTTGrid* grid, TTTPlayerMove move )
{
   assert(grid != NULL);
   grid->setGridLocation( move );
}
static void 
undoMove( TTTGrid* grid, TTTPlayerMove move )
{
   assert(grid != NULL);
   grid->setGridLocation( TTTPlayerMove( move.m_j, move.m_i, Empty ));
}

static NodeState
getRandomFromSet(const AIMOVES& moves)
{
   assert(!moves.empty());
   CHUint idx = rand() % (CHUint)moves.size();
   return moves[idx];
}

static AIMOVES
getSetOfValues(const AIMOVES& moves, bool highest)
{
   assert(!moves.empty());
   int value = highest ? INT_MIN : INT_MAX;
   CHUint sz = (CHUint)moves.size();

   for(CHUint i=0; i<sz; ++i)
      if(highest && moves[i].m_value > value ||
         !highest && moves[i].m_value < value)
         value = moves[i].m_value;

   AIMOVES ret;
   for(CHUint i=0; i<sz; ++i)
      if(moves[i].m_value == value)
         ret.push_back(moves[i]);

   return ret;
}

static NodeState
bestMoveFromSet(const AIMOVES& moves)
{
//#define RANDOM
#ifdef RANDOM
   AIMOVES randmoves = getSetOfHighestValues(moves); 
   return getRandomFromSet(randmoves);
#else
   assert(!moves.empty());
   int value = INT_MIN;
   int retIdx = 0;
   CHUint sz = (CHUint)moves.size();
   for( CHUint i=0; i<sz; ++i)
      if( moves[i].m_value >= value )
      {
         retIdx = i;
         value = moves[i].m_value;
      }
   return moves[retIdx];
#endif
}

NodeState
TTTAIUtils::getMinMove(const TTTGrid& grid, const TTTPlayerMove& previousMove, 
                       TTTPlayer turn, const NodeState& nodeToBeat, CHUint depth, bool root)
{
   bool isRoot = root;
   root = false;

   assert( depth >= 1);
   if(depth == 1)
   {
      AIMOVES moves = getPossibleMovesWithValues(grid,turn);
      return bestMoveFromSet(moves);
   }
   --depth;

   AIMOVES moves = getPossibleMoves(grid, turn);
   if( moves.empty() )
   {
      return NodeState(nodeToBeat);
   }
   else if( moves.size() == 1 )
      return moves[0];
   else
   {
		NodeState bestNode;
		bestNode.m_value = INT_MIN;
      AIMOVES finalMoves;
		TTTPlayer opponent = TTTGameState::getOtherPlayer(turn);

		TTTGrid* newGrid = grid.clone();
      CHUint numMoves = (CHUint)moves.size();
      for( CHUint i=0; i< numMoves; ++i )
      {			
			makeMove( newGrid, moves[i].m_move );
         NodeState tempNode = getMaxMove(*newGrid, moves[i].m_move, opponent, bestNode, depth, false);
			undoMove( newGrid, moves[i].m_move );

         if(tempNode.m_value > nodeToBeat.m_value)
            return NodeState( tempNode.m_value, previousMove );

			if(tempNode.m_value >= bestNode.m_value)
         {
            if(tempNode.m_value > bestNode.m_value)
               finalMoves.clear();

            bestNode.m_move = moves[i].m_move;
            bestNode.m_value = tempNode.m_value;
            finalMoves.push_back(bestNode);
         }
      }
		delete newGrid; newGrid = NULL;

		if(! isRoot )
         return NodeState(bestNode.m_value, previousMove);

		return getRandomFromSet(finalMoves);
   }
}

NodeState
TTTAIUtils::getMaxMove(const TTTGrid& grid, const TTTPlayerMove& previousMove, 
                       TTTPlayer turn, const NodeState& nodeToBeat, CHUint depth, bool root)
{
   assert( depth >= 1);
   bool isRoot = root;
   root = false;

   AIMOVES moves = getPossibleMoves(grid, turn);
   if( moves.empty() )
   {
      return NodeState(nodeToBeat);
   }
   else if( moves.size() == 1 )
      return moves[0];
   else
   {
		NodeState bestNode;
		bestNode.m_value = INT_MAX;
		TTTPlayer opponent = TTTGameState::getOtherPlayer(turn);

      AIMOVES finalMoves;
		TTTGrid* newGrid = grid.clone();
      CHUint numMoves = (CHUint)moves.size();
      for( CHUint i=0; i< numMoves; ++i )
      {
			makeMove( newGrid, moves[i].m_move );
         NodeState tempNode = getMinMove(*newGrid, moves[i].m_move, opponent, bestNode, depth, false);
			undoMove( newGrid, moves[i].m_move );
         
         if(tempNode.m_value > nodeToBeat.m_value)
            return NodeState( tempNode.m_value, previousMove );
         
			if(tempNode.m_value <= bestNode.m_value)
         {
            if(tempNode.m_value < bestNode.m_value)
               finalMoves.clear();

            bestNode.m_move = moves[i].m_move;
				bestNode.m_value = tempNode.m_value;
            finalMoves.push_back(bestNode);
         }
      }
		delete newGrid; newGrid = NULL;

      if(! isRoot )
         return NodeState(bestNode.m_value, previousMove);

		return getRandomFromSet(finalMoves);
   }
}

NodeState
TTTAIUtils::getMaxMoveMultiThreaded(const TTTGrid& grid, 
                                    const TTTPlayerMove& previousMove, 
                                    TTTPlayer turn, 
                                    const NodeState& nodeToBeat, 
                                    unsigned int depth)
{
   assert( m_threadPool != NULL);
   assert( depth >= 1);

   AIMOVES moves = getPossibleMoves(grid, turn);
   if( moves.empty() )
   {
      return NodeState(nodeToBeat);
   }
   else if( moves.size() == 1 )
      return moves[0];
   else
   {
		NodeState bestNode;
		bestNode.m_value = INT_MAX;
		TTTPlayer opponent = TTTGameState::getOtherPlayer(turn);

      std::vector<TaskObject*> taskObjects;  
      CHUint numMoves = (CHUint)moves.size();

      for( CHUint i=0; i< numMoves; ++i )
      {
         TTTGrid* newgrid = grid.clone();
         makeMove( newgrid, moves[i].m_move );

         TaskObject* taskObject = new TaskObject(newgrid, moves[i], opponent);
         ThinkTask* task = new ThinkTask(taskObject, depth);
         m_threadPool->addTask( task );

         taskObjects.push_back(taskObject);
      }
      m_threadPool->finishTasks();
     
      // record moves
      AIMOVES analyzedMoves;
      numMoves = (CHUint)taskObjects.size();
      for(CHUint i=0; i<numMoves; ++i)      
      {
         analyzedMoves.push_back( taskObjects[i]->m_theMove );
         delete taskObjects[i];
      }
      taskObjects.clear();      
                        
		return getRandomFromSet(getSetOfValues(analyzedMoves, false));
   }
      
}


TTTPlayerMove 
TTTAIUtils::getBestMove(TTTGameState& gameState, unsigned int depth)
{
   NodeState nodeToBeat;
	nodeToBeat.m_value = INT_MAX;

   NodeState bestMove = TTTAIUtils::getMaxMove(gameState.grid(), TTTPlayerMove::EMPTY, gameState.turn(), nodeToBeat, depth, true);   
   return bestMove.m_move;
}

TTTPlayerMove 
TTTAIUtils::getBestMoveMultiThreaded(TTTGameState& gameState, unsigned int depth)
{
   NodeState nodeToBeat;
	nodeToBeat.m_value = INT_MAX;

   NodeState bestMove = TTTAIUtils::getMaxMoveMultiThreaded(gameState.grid(), TTTPlayerMove::EMPTY, gameState.turn(), nodeToBeat, depth);   
   return bestMove.m_move;
}