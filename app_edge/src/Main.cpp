


#include "Renderable.h"
#include "World.h"
#include "EdgeGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 400,300,1600,600);

	theWorld.SetGameManager(&theEdgeGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
