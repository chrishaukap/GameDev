
#include "stdafx.h"

#include "Infrastructure/Renderable.h"
#include "Infrastructure/World.h"
#include "EdgeGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize();

	theWorld.SetGameManager(&theEdgeGame);
	
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
