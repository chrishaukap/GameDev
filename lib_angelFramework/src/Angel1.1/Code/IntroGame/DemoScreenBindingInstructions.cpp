#include "stdafx.h"
#include "DemoScreenBindingInstructions.h"

#include "World.h"
#include "TextActor.h"

DemoScreenBindingInstructions::DemoScreenBindingInstructions()
{
}

void DemoScreenBindingInstructions::Start()
{
	//Some TextActors pointing you to other files that do cool stuff. 
	TextActor *t = new TextActor("Console", "(While we're looking at config files, check out autoexec.cfg.");
	t->SetPosition(0, 3.5);
	t->SetAlignment(TXT_Center);
	TextActor *t2 = new TextActor("Console", "It shows how to do controller and keyboard binding, and sets some console");
	t2->SetPosition(0, 2);
	t2->SetAlignment(TXT_Center);
	TextActor *t3 = new TextActor("Console", "variables that get used in code.)");
	t3->SetPosition(0, .5);
	t3->SetAlignment(TXT_Center);

	theWorld.Add(t);
	theWorld.Add(t2);
	theWorld.Add(t3);






	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenBindingInstructions.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(t3);
	#pragma endregion
}