
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"
#include "TetrisGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 400,300,200,900);

	theWorld.SetGameManager(&theTetrisGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
