
#include "stdafx.h"

#include "Renderable.h"
#include "World.h"

#include "GridActor.h"
#include "TheEndGameManager.h"
#include "ConfigReader.h"
#include "LifeTrophies.h"
#include "LifeChoicePoints.h"

int main(int argc, char* argv[])
{
	const char file[] = "Config.txt";
	ConfigReader::readConfigFile(file);

	theWorld.Initialize(argc, argv, 1024, 768, 200, 200, "The End");
	theWorld.Add(new GridActor());	   
	theWorld.SetGameManager(&theEndGame);


	theWorld.StartGame();
	
	thePointTracker.Destroy();
	theLifeTrophies.Destroy();

	// any cleanup can go here
	theWorld.Destroy();

	
	return 0;
}
