
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"
#include "Log.h"

#include "DemoGame.h"

int main(int argc, char* argv[])
{
	// get things going
	//  additional parameters:
	//		int windowWidth			default: 1024
	//		int windowHeight		default: 768
	//		std::string windowName	default: "Angel Engine"
	theWorld.Initialize(argc, argv);


	// DEMO CODE

	// How to log stuff to a file in the Logs directory. 
	FileLog log(FileLog::MakeLogFileName("Launch"));
	log.Log("testing some stuff" );
	log.Log("did this work?");

	//We're going to be using physics, but just with the defaults.
	theWorld.SetupPhysics();

	// DemoGame inherits from Renderable, implementing its own update and render functions.
	DemoGame *game = new DemoGame();
	// The Game Manager will always get updated before all the actors.
	theWorld.SetGameManager(game);

	// END DEMO CODE

	// Do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();

	// any cleanup can go here
	theWorld.Destroy();
	delete game;


	return 0;
}
