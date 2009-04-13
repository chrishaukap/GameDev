
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"
#include "GridActor.h"
#include "DemoGameManager.h"

int main(int argc, char* argv[])
{
	// get things going
	//  additional parameters:
	//		int windowWidth			default: 1024
	//		int windowHeight		default: 768
	//		std::string windowName	default: "Angel Engine"
	theWorld.Initialize(argc, argv);

	//we're going to be using the built-in physics
	theWorld.SetupPhysics();

	//add the default grid so we can see where our Actors are in the world
	theWorld.Add(new GridActor());
	
	//theDemoGame is our DemoGameManager singleton
	// all the cool stuff happens there -- check it out. 
	theWorld.SetGameManager(&theDemoGame);
	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
