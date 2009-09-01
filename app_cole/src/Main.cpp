#include "Renderable.h"
#include "World.h"
#include "ColeGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 400,300,1600,500);

	theWorld.SetGameManager(&theColeGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
