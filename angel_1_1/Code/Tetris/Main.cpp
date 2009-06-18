
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"
#include "GridActor.h"
#include "TetrisGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 800,600,100,100);

	theWorld.SetGameManager(&theTetrisGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
