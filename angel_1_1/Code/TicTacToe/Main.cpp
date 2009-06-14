#include "stdafx.h"

#include "World.h"
#include "MathUtil.h"
#include "TicTacToeGameManager.h"
#include "minimax.h"
#include <time.h>
#include "TTTAIUtils.h"

using namespace TicTacToe;
int main(int argc, char* argv[])
{
   
	srand( (unsigned int)time(NULL) ); 
   TTTAIUtils::init();

	const unsigned int scrnWidth = 400;
	const unsigned int scrnHeight = 300;

	theWorld.Initialize(argc, argv, scrnWidth, scrnHeight, 1600, 900, "TicTacToe");
   theWorld.SetGameManager( &theTTTGameMngr );
	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

   
   TTTAIUtils::uninit();
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
