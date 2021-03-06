
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"
#include "GridActor.h"
#include "VineGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize(argc, argv, 800, 600, 100, 100);
	theWorld.SetupPhysics();

   GridActor* gridActor = new GridActor(Color3f(0.85f, 0.85f, 0.85f), Color3f(0.5f,0.5f,0.5f), 1.0f );
	theWorld.Add(gridActor);
	 
	theWorld.SetGameManager(&theVineGame);
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
