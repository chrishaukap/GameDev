
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"

#include "GridActor.h"

int main(int argc, char* argv[])
{
	// get things going
	//  additional parameters:
	//		int windowWidth			default: 1024
	//		int windowHeight		default: 768
	//		std::string windowName	default: "Angel Engine"
	theWorld.Initialize(argc, argv);
	
	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor());

	//YOUR GAME SETUP CODE HERE
	
	

	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
