#include "stdafx.h"
#include "minimax.h"
#include "mytimer.h"
#include "TTTAIUtils.h"

#include <time.h>

using namespace CDH;
using namespace TicTacToe;

static void writeTimeToFile(const char* filename, DWORD tickcount)
{
   FILE* file = fopen(filename, "a");	   
   char buf[100];
	static int counter = 0;
   sprintf( buf, "[%d] tickCount: %d\n", ++counter, tickcount );
   fwrite(buf, strlen(buf), sizeof(char), file);
   fclose(file);
}

static mytimer *g_timer = NULL;
TTTPlayerMove 
MiniMaxTree::getBestMove(TTTGameState& gameState, unsigned int depth)
{		
#if _DEBUG
	if(g_timer == NULL)
      g_timer = new mytimer();
	g_timer->start();
#endif

   TTTPlayerMove bestMove;
   //bestMove = TTTAIUtils::getBestMoveMultiThreaded(gameState, depth);
   bestMove = TTTAIUtils::getBestMove(gameState, depth);

#ifdef _DEBUG
	g_timer->end();

	writeTimeToFile("multi-threaded.txt", g_timer->tickCount());
	//writeTimeToFile("single-threaded.txt", g_timer->tickCount());

/*
#ifdef MULTI_THREADED
	writeTimeToFile("multithreaded.txt", g_timer->tickCount());
#else
	writeTimeToFile("singlethreaded.txt", g_timer->tickCount());
#endif
*/
#endif
	return bestMove;
}
