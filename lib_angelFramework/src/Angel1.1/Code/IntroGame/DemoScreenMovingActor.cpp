#include "stdafx.h"
#include "DemoScreenMovingActor.h"

#include "World.h"
#include "Camera.h"
#include "Xbox360Input.h"

DemoScreenMovingActor::DemoScreenMovingActor()
{
}

void DemoScreenMovingActor::Start()
{
	//Set up the actor
	a = new Actor();
	a->SetSize(4.0f);
	a->SetColor(1.0f, 1.0f, 0.0f, 0.5f);
	theWorld.Add(a);





	//Demo housekeeping below this point. 
	#pragma region Demo Housekeeping
	t = new TextActor("Console", "This Actor gets moved around by the left thumbstick. Try it.");
	t->SetPosition(0, 3.5);
	t->SetAlignment(TXT_Center);
	t2 = new TextActor("Console", "Press [B] to rotate him.");
	t2->SetPosition(0, -4);
	t2->SetAlignment(TXT_Center);
	t3 = new TextActor("Console", "(The camera is a movable Actor, too -- right thumbstick.)");
	t3->SetPosition(0, -8);
	t3->SetAlignment(TXT_Center);
	theWorld.Add(t);
	theWorld.Add(t2);
	theWorld.Add(t3);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenMovingActor.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(t3);
	_objects.push_back(a);
	#pragma endregion
}

void DemoScreenMovingActor::Update(float dt)
{
	//Update position based on thumbstick
	Vec2i movementL = theController.GetLeftThumbstick(); //returns a Vector2 where
														 //each component ranges from
														 //-32768 to 32768

	//NOTE: by default, the thumbstick has a dead zone around the middle where
	//  it will report position as 0. This prevents jitter when the stick isn't
	//  being touched. 

	Vector2 position;
	if (movementL.X)
	{
		// Scale the thumbstick down to reasonable proportions and make the range of
		//  movement 3
		position.X = 3.0f * ((float)movementL.X / 32768.0f);
	}
	else
	{
		position.X = 0.0f;
	}
	if (movementL.Y)
	{
		position.Y = 3.0f * ((float)movementL.Y / 32768.0f);
	}
	else
	{
		position.Y = 0.0f;
	}

	//Update the position with our calculated values. 
	a->SetPosition(position);

	//Every tick, update the rotation if B is held down
	if (theController.IsBButtonDown())
	{
		a->SetRotation(a->GetRotation() + (90.0f * dt)); //90 degrees per second
		if (a->GetRotation() > 360.0f)
		{
			a->SetRotation(a->GetRotation() - 360.0f);
		}
	}


	//Doing the same math we did above for the regular Actor, but this
	//  time applying the position changes to theCamera singleton. 
	Vec2i movementR = theController.GetRightThumbstick();
	Vector2 camPos;
	if (movementR.X)
	{
		camPos.X = 5.0f * ((float)movementR.X / 32768.0f);
	}
	else
	{
		camPos.X = 0.0f;
	}
	if (movementR.Y)
	{
		camPos.Y = 5.0f * ((float)movementR.Y / 32768.0f);
	}
	else
	{
		camPos.Y = 0.0f;
	}

	theCamera.SetPosition(camPos.X, camPos.Y);
}