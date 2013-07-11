#include "stdafx.h"

#include "AdventurerGame.h"

int main(int argc, char* argv[])
{
   const unsigned int screenWidth = 1024;
   const unsigned int screenHeight = 768;
	//.Initialize(600,400, "Adventuring is Difficult");
	theWorld.Initialize(screenWidth ,screenHeight , "Adventuring is Difficult");
	
	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor(), -1);
	
	//YOUR GAME SETUP CODE HERE
	
	theWorld.SetGameManager(&theGame);
   theGame.Init(screenWidth,screenHeight );
	
	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
