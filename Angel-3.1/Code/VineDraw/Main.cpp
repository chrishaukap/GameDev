
#include "stdafx.h"

#include "Infrastructure/Renderable.h"
#include "Infrastructure/World.h"
#include "Actors/GridActor.h"
#include "VineGameManager.h"

int main(int argc, char* argv[])
{
	theWorld.Initialize();
	theWorld.SetupPhysics();

   GridActor* gridActor = new GridActor( /*Color(0.85f, 0.85f, 0.85f), Color(0.5f,0.5f,0.5f), 1.0f*/ );
	theWorld.Add(gridActor);
	 
	theWorld.SetGameManager(&theVineGame);
	theWorld.StartGame();
	theWorld.Destroy();
	
	return 0;
}
