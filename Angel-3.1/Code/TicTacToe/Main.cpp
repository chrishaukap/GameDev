#include "stdafx.h"

#include "Infrastructure/World.h"
#include "Util/MathUtil.h"
#include "TicTacToeGameManager.h"
#include "minimax.h"
#include <time.h>
#include "TTTAIUtils.h"

using namespace TicTacToe;
int main(int argc, char* argv[])
{
   
	srand( (unsigned int)time(NULL) ); 
   TTTAIUtils::init();
   
   unsigned int width = 1200; 
   unsigned int height = 800;
   theTTTGameMngr.init(width, height);
	theWorld.Initialize(width, height);
   theWorld.SetGameManager( &theTTTGameMngr );
	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

   
   TTTAIUtils::uninit();
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
