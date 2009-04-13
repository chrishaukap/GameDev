#include "stdafx.h"
#include "DemoScreenParticleActors.h"

#include "ParticleActor.h"
#include "World.h"
#include "Xbox360Input.h"
#include "ActorFactory.h"

DemoScreenParticleActors::DemoScreenParticleActors()
{
	_isActive = false;
}

void DemoScreenParticleActors::Start()
{
	// Create the particle actor via the Actor Definition system (.adf files)
	theActorFactory.BeginActor("particle_demo", 0);
		theActorFactory.GetCurrentActor()->SetPosition(0.0f, 0.0f);
		pa = (ParticleActor*)theActorFactory.GetCurrentActor();
	theActorFactory.EndActor();

	_isActive = true; //lets the mouse events know that they should care

	//Demo housekeeping below this point. 
	#pragma region Demo Housekeeping
	t = new TextActor("Console", "Here's a ParticleActor. (Try moving and clicking the mouse!)");
	t->SetPosition(0, 3.5);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);
	t2 = new TextActor("Console", "Press [B] to change its properties.");
	t2->SetPosition(0, 2.5);
	t2->SetAlignment(TXT_Center);
	theWorld.Add(t2);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenParticleActors.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(pa);
	#pragma endregion
}

void DemoScreenParticleActors::Update(float dt)
{
	//This is the same kind of input processing we did in DemoScreenSimpleActor.cpp,
	//  but here we're playing with the particles. 
	if (theController.IsButtonDown(XINPUT_GAMEPAD_B))
	{
		pa->SetColor(1.0f, 0.0f, 0.0f, .25f);
		pa->SetSize(0.5f, 0.5f);
		pa->SetGravity( Vector2(0.0f, 0.0f) );
		pa->ClearSpriteInfo();
		t->SetDisplayString("Now it's red and translucent. Press [Y].");
		t2->SetDisplayString("");
	}
	if (theController.IsButtonDown(XINPUT_GAMEPAD_Y))
	{
		pa->SetColor(0,0,1,1); 		
		pa->SetSize(0.5f, 0.5f);
		pa->SetGravity( Vector2(0.0f, -8.0f) );
		pa->ClearSpriteInfo();
		t->SetDisplayString("Now it's blue and has density. Press [X] for the coolest thing ever.");
		t2->SetDisplayString("");
	}
	if (theController.IsButtonDown(XINPUT_GAMEPAD_X))
	{
		pa->SetSize(4.0f, 4.0f);
		pa->SetColor(1,1,1,1);
		pa->SetGravity( Vector2(0.0f, 0.0f) );
		pa->LoadSpriteFrames("Resources\\Images\\numbers\\angel_01.png");
		pa->PlaySpriteAnimation(0.5f, SAT_Loop, 0, 4);

		t->SetDisplayString("That's right, animated textures. You love it.");
		t2->SetDisplayString("");
	}
}

void DemoScreenParticleActors::MouseMotionEvent(Vec2i screenCoordinates)
{
	if (_isActive) //we only want to move around if this screen is currently active
	{
		pa->SetPosition(MathUtil::ScreenToWorld(screenCoordinates.X, screenCoordinates.Y));
	}
}

void DemoScreenParticleActors::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
	if (_isActive)
	{
		ParticleActor *oneOff = (ParticleActor*)theActorFactory.CreateActor("particle_demo", "", 0, NULL);
		oneOff->SetColor(0, 0, 1);
		oneOff->SetSprite("Resources\\Images\\triangle.png");
		//We can set the position to where the mouse click happened.
		oneOff->SetPosition(MathUtil::ScreenToWorld(screenCoordinates.X, screenCoordinates.Y));

		//The system will remove itself from the world and deallocate its memory
		//  when the lifetime ends. (If it's 0.0, it's assumed to be infinite.)
		oneOff->SetSystemLifetime(1.0f);
	}
}

void DemoScreenParticleActors::Stop()
{
	DemoScreen::Stop();

	// Now that DemoScreen cleaned up...
	_isActive = false;
}
