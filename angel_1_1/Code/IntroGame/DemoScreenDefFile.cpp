#include "stdafx.h"
#include "DemoScreenDefFile.h"

#include "World.h"
#include "ActorFactory.h"

void CustomFunc(Actor* a)
{
	a->SetRotation(45.0f);
	a->SetPosition(0, -1);
}

DemoScreenDefFile::DemoScreenDefFile()
{
}

void DemoScreenDefFile::Start()
{

	//CreateActor loads up an Actor Definition file and makes the actor from it
	a = theActorFactory.CreateActor(
		"simple_actor", //the file to load from -- must be located in Config\ActorDef and end with ".adf"
		"PurpleActor",	//the desired name of the actor
		0,				//the render layer in which to put the actor (optional)
		CustomFunc		//a custom initialization function if you want one (optional, can be NULL)
	);
	//theWorld.Add(a);   // NOTE: CreateActor adds it to the world for you






	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	t = new TextActor("Console", "This Actor was placed using an ActorDef file.");
	t->SetPosition(0, 4.5);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);
	t2 = new TextActor("Console", "You can be data-driven if you want to!");
	t2->SetPosition(0, 3.5);
	t2->SetAlignment(TXT_Center);
	theWorld.Add(t2);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenDefFile.cpp, simple_actor.adf");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(a);
	#pragma endregion
}

