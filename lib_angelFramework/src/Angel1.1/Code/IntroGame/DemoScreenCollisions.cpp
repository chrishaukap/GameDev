#include "stdafx.h"
#include "DemoScreenCollisions.h"

#include "World.h"
#include "Xbox360Input.h"

DemoScreenCollisions::DemoScreenCollisions()
{
	//This can cause a skip if you try to do it on demand. 
	//  We recommend you load all your sounds in advance if possible. 
	sound = theSound.LoadSound("Resources\\Sounds\\sprong.wav");
}

void DemoScreenCollisions::Start()
{
	//so we can get notifications of collisions
	theWorld.AddCollisionListener(this);

	//Set up the PhysicsActors to collide
	p1 = new PhysicsActor();
	p1->SetSize(1.0f);
	p1->SetColor(1.0f, 0.0f, 1.0f);
	p1->InitPhysics(0.8f, 0.5f, 0.7f);

	p2 = new PhysicsActor();
	p2->SetPosition(0.0f, -11.0f);
	p2->SetSize(30.0f, 5.0f);
	p2->SetColor(0.0f, 1.0f, 0.0f);
	p2->InitPhysics(0.0f, 0.1f);

	theWorld.Add(p1);
	theWorld.Add(p2);






	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	t = new TextActor("Console", "This example looks similar, but is responding to collisions with sound.");
	t->SetPosition(0.0f, 3.5f);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenCollisions.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(p1);
	_objects.push_back(p2);
	#pragma endregion
}

void DemoScreenCollisions::Stop()
{
	//Make sure to unsubscribe from collision events when
	//  you get destroyed, or the world will try and call
	//  OnCollision() on bad memory. 
	theWorld.RemoveCollisionListener(this);
	DemoScreen::Stop();
}

void DemoScreenCollisions::Update(float dt)
{
	if (theController.IsBButtonDown())
	{
		//punch it upwards
		p1->ApplyForce(Vector2(0, 20.0f), Vector2());
	}
}

void DemoScreenCollisions::OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation)
{
	//This delivers the actual shapes, so you have to check the Actors you care about
	// (Yes, it's kind of naive at the moment and everyone hears about everything.
	//  This is something to fix for Angel 2.0. :-) )

	if (s1->GetBody() != p1->GetBody() && s1->GetBody() != p2->GetBody())
	{
		return;
	}
	if (s2->GetBody() != p1->GetBody() && s2->GetBody() != p2->GetBody())
	{
		return;
	}

	// Here's how you play sound! 
	theSound.PlaySound(
		sound, //pointer to your sound object 
		1.0f,  //desired volume
		false, //whether it should loop
		0	   //flags -- the only one we care about right now
			   //  is PSF_STOP_SOUND, which will just kill the
			   //  sound if it's playing
	);

	//The sound system is pretty simple at the moment. Need something
	//  better? You've got the source code. :-) 

}