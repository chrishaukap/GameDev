#include "stdafx.h"
#include "DemoScreenLayeredCollisionLevelFile.h"

#include "World.h"
#include "ActorFactory.h"
#include "TagCollection.h"

DemoScreenLayeredCollisionLevelFile::DemoScreenLayeredCollisionLevelFile()
{
}

void DemoScreenLayeredCollisionLevelFile::Start()
{
	//Loads the file from Config\ActorDef\layeredcollisionlevel_demo.lvl
	LoadLevel("layeredcollisionlevel_demo");

	//All the magic happens in the level file!





	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	t2 = new TextActor("Console", "These new Actors were assigned layers in the level file.");
	t2->SetPosition(0, 5.5);
	t2->SetAlignment(TXT_Center);
	theWorld.Add(t2, 10);
	t3 = new TextActor("Console", "Layer names are defined in autoexec.cfg (note variables starting with \"layer_\"),");
	t3->SetPosition(0, 4.5);
	t3->SetAlignment(TXT_Center);
	theWorld.Add(t3, 10);
	t4 = new TextActor("Console", "and assigned to Actors by ActorFactorySetLayerName <layerName>.");
	t4->SetPosition(0, 3.5);
	t4->SetAlignment(TXT_Center);
	theWorld.Add(t4, 10);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenLayeredCollisionLevelFile.cpp, layeredcollisionlevel_demo.lvl,");
	TextActor *fileLoc2 = new TextActor("ConsoleSmall", "      autoexec.cfg");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 745));
	fileLoc->SetColor(.3f, .3f, .3f);
	fileLoc2->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc2->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc, 10);
	theWorld.Add(fileLoc2, 10);
	_objects.push_back(fileLoc);
	_objects.push_back(fileLoc2);
	_objects.push_back(t2);
	_objects.push_back(t3);
	_objects.push_back(t4);
	ActorSet spawnedActors = tagList.GetObjectsTagged("spawned");
	ActorSet::iterator it = spawnedActors.begin();
	while (it != spawnedActors.end())
	{
		_objects.push_back(*it);
		it++;
	}
	#pragma endregion
}

