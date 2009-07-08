


#include "Renderable.h"
#include "World.h"
#include "EdgeGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 400,300,200,200);

	theWorld.SetGameManager(&theEdgeGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
