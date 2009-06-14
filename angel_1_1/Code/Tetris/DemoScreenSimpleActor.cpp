#include "stdafx.h"
#include "DemoScreenSimpleActor.h"

#include "World.h"
#include "Xbox360Input.h"

DemoScreenSimpleActor::DemoScreenSimpleActor()
{
}

void DemoScreenSimpleActor::Start()
{
	//Adding an actor to the world is very simple
	a = new Actor();
	a->SetSize(5.0f);
	a->SetColor(0,0,0);

	theWorld.Add(a);





	//Demo housekeeping below this point. 
	#pragma region Demo Housekeeping
	t = new TextActor("Console", "Here's a simple Actor. (Press [B] to change it.)");
	t->SetPosition(0, 3.5);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenSimpleActor.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(a);
	#pragma endregion
}

void DemoScreenSimpleActor::Update(float dt)
{
	//Here we're doing some input processing and altering the Actor based on it. 
	if (theController.IsBButtonDown())
	{
		a->SetColor(1.0f, 0.0f, 1.0f, .5f); //R, G, B, A (there are also Color3f and Color4f structs)
		a->ClearSpriteInfo(); //removes any texture that might have been assigned
		t->SetDisplayString("Now it's purple and translucent. Press [Y] to give it a texture.");
	}
	if (theController.IsYButtonDown())
	{
		a->SetColor(1,1,1,1); //(white and opaque so the texture comes through fully)
		a->ClearSpriteInfo();
		a->SetSprite("Resources\\Images\\angel.png"); //Loads any image supported by FreeImage (see docs)
		t->SetDisplayString("Pretty easy. You can do animations as well. Press [X] to check it out.");
	}
	if (theController.IsXButtonDown())
	{
		a->SetColor(1,1,1,1);
		a->LoadSpriteFrames("Resources\\Images\\numbers\\angel_01.png");
		a->PlaySpriteAnimation(
			0.5f,			//amount of time between frames
			SAT_Loop,		//other options are SAT_PingPong and SAT_OneShot
			0,				//starting frame
			4,				//ending frame
			"AngelNumbers"	//name of the animation so you can get the event when it finishes
		);
		t->SetDisplayString("You can also change the speed and looping behavior if you want. ([A] to move on.)");
	}
}