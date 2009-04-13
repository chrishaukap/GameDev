#include "StdAfx.h"
#include "DemoGame.h"
#include "World.h"
#include "TextRendering.h"
#include "Xbox360Input.h"
#include "DemoActor.h"
#include "Renderable.h"
#include "PhysicsActor.h"
#include "PhysicsEventActor.h"
#include "PlaySoundCollisionResponse.h"
#include "Sound.h"

#include "ActorFactory.h"	//rb - Used for loading ADF files from code.

DemoGame::DemoGame()
{
	_simTime = 0.0;

	_fullScreen = new FullScreenActor();
	_fullScreen->SetSprite("Resources\\Images\\garden.jpg");

	_demo1 = new DemoActor();
	_demo2 = new DemoActor();
	_demo3 = new DemoActor();
	_demo4 = new DemoActor();
	_demo5 = new DemoActor();
	_demo6 = new DemoActor();
	_physGround = new PhysicsEventActor();
	_physDemo1 = new PhysicsEventActor();
	_physDemo2 = new PhysicsActor();

	theWorld.Add(_fullScreen);
	theWorld.Add(_demo1, RL_World);
	theWorld.Add(_demo2, RL_World);
	theWorld.Add(_demo3, RL_World);
	theWorld.Add(_demo4, RL_World);
	theWorld.Add(_demo5, RL_World);
	theWorld.Add(_demo6, RL_World);
	theWorld.Add(_physDemo1, RL_World);
	theWorld.Add(_physDemo2, RL_World);
	//theWorld.Add(_physGround, RL_World);

	_demo1->SetName("Plastic");
	_demo1->Tag("purple");
	_demo1->SetColor(.5, 0.0, .75);
	_demo1->SetSize(2.5f);
	_demo1->SetAlpha(0.5f);
	_demo1->SetPosition(5.0f, 3.0f);

	_demo2->SetName("RightWoodPiece");
	_demo2->SetSize(3.14f);
	_demo2->SetPosition(3.0f, -5.0f);
	_demo2->LoadSpriteFrames("Resources\\Images\\numbers\\numbers_01.jpg");
	_demo2->PlaySpriteAnimation(0.5f, SAT_OneShot, 0, 16, "MeleeSwing");

	_demo3->SetName("LeftWoodPiece");
	_demo3->SetSize(3.0f);
	_demo3->SetPosition(-3.0f, 5.0f);
	_demo3->SetSprite("Resources\\Images\\woodfloor.png");

	_demo4->SetSize(3.0f);
	_demo4->SetPosition(-1.0f, -2.0f);
	_demo4->LoadSpriteFrames("Resources\\Images\\numbers\\numbers_01.jpg");
	_demo4->PlaySpriteAnimation(0.2f, SAT_Loop, 0, 5);

	_demo5->SetSize(3.0f);
	_demo5->SetPosition(-2.0f, -2.0f);
	_demo5->SetSprite("Resources\\Images\\woodfloor.png");

	_demo6->SetSize(3.0f);
	_demo6->SetPosition(-2.0f, -1.0f);
	_demo6->SetColor(1.0f, 0.0f, 0.0f);
	_demo6->SetAlpha(0.5f);

	_physGround->SetSize(20.f);
	_physGround->SetPosition(0.f, -18.f);
	_physGround->SetColor(0.f, 0.8f, 0.2f);
	_physGround->SetCollisionId( "ground" );
	//_physGround->InitPhysics(0.f, 0.1f); // no density, 0.1f friction

	_physDemo1->SetSize(3.0f, 2.0f);
	_physDemo1->SetPosition(4.0f, 8.0f);
	_physDemo1->SetRotation(-50.f);
	_physDemo1->SetColor(0.2f, 0.2f, 1.0f);
	StringList sndColResInput;
	sndColResInput.push_back( "Resources\\Sounds\\b1.wav" );
	CollisionResponse* pSoundCollisionResponse = PlaySoundCollisionResponse::FactoryMethod( sndColResInput );
	_physDemo1->RegisterCollisionResponse( "ground", pSoundCollisionResponse );
	_physDemo1->InitPhysics(0.4f, 0.5f, 0.7f); // box is default

	// Physics actors support box or circle shapes.  The physics system supports arbitrary polygons and
	// bodies made of aggregate shapes as well - derive from PhysicsActor to use those types
	_physDemo2->SetSize(1.5f);
	_physDemo2->SetPosition(-4.0f, 8.0f);
	_physDemo2->SetSprite("Resources\\Images\\8ball.png");
	_physDemo2->InitPhysics(0.8f, 0.7f, 0.0f, PhysicsActor::SHAPETYPE_CIRCLE);

	RegisterFont("Resources\\Fonts\\Inconsolata.otf", 24, "Console");
	RegisterFont("Resources\\Fonts\\Inconsolata.otf", 18, "ConsoleSmall");

	_firstSound = theSound.LoadSound("Resources\\Sounds\\b1.wav");
	
	theWorld.SetSideBlockers(true, 0.5f);

	theWorld.AddCollisionListener(this);
}

DemoGame::~DemoGame()
{
	theWorld.RemoveCollisionListener(this);

	delete _demo1;
	delete _demo2;
	delete _demo3;
	delete _demo4;
	delete _demo5;
	delete _demo6;
	delete _physGround;
	delete _physDemo1;
	delete _physDemo2;
}

void DemoGame::Update(float dt)
{
	_simTime += dt;

	// We're in the world now, so we will get updated automatically.
	//_demo1->update(dt);
	//_demo2->update(dt);
	//_demo3->update(dt);

	if (theController.IsStartButtonDown())
	{
		theWorld.StartSimulation();
	}
	if (theController.IsBackButtonDown())
	{
		theWorld.StopSimulation();
	}

	// Test dynamically adjusting the layers.
	if (theController.IsButtonDown(XINPUT_GAMEPAD_DPAD_UP))
	{
		theWorld.UpdateLayer(_demo4, RL_World);
		theWorld.UpdateLayer(_demo5, RL_Foreground);
		theWorld.UpdateLayer(_demo6, RL_Background);
	}
	else if (theController.IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		theWorld.UpdateLayer(_demo4, RL_Background);
		theWorld.UpdateLayer(_demo5, RL_Foreground);
		theWorld.UpdateLayer(_demo6, RL_World);
	}

	theController.SetLeftVibration(theController.GetLeftTrigger());
	theController.SetRightVibration(theController.GetRightTrigger());

	// physics test
	if (theController.IsAButtonDown())
	{
		// apply force from current side, so that you have a chance of keeping it onscreen
		float side = 2.0f*_physDemo1->GetPosition().X;
		_physDemo1->ApplyForce(Vector2(0.f, 20.f), Vector2(side, 0.0f));

		// apply force from current side, so that you have a chance of keeping it onscreen
		side = 2.0f*_physDemo2->GetPosition().X;
		_physDemo2->ApplyForce(Vector2(0.f, 20.f), Vector2(side, 0.0f));

		// can also access body directly
		//_physDemo1->GetBody()->WakeUp();
		//_physDemo1->GetBody()->SetLinearVelocity(b2Vec2(1.0f, 0.0f));
	}
}

void DemoGame::Render()
{
	// This is a cheat -- we should do this as a system update. 
	if (theController.IsButtonDown(XINPUT_GAMEPAD_START))
	{		
		theWorld.StartSimulation();
	}

	glColor4f(0,0,1,.5f);
	DrawGameText("Text can also be rendered at an angle, and with colors and alpha.", "Console", 20, 100, 45);
	glColor4f(0,0,0,1);
	char timeString[200];
	char simTimeString[200];
	sprintf_s(timeString, "Program has been running for %3.1f seconds.", (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	sprintf_s(simTimeString, "Simulation has been running for %3.1f seconds.", _simTime);
	DrawGameText(timeString, "ConsoleSmall", 30, 750);
	DrawGameText(simTimeString, "ConsoleSmall", 30, 720);
}

void DemoGame::OnCollision(b2Shape* s1, b2Shape* s2, float normalImpulse, Vector2& hitLocation)
{
	// only care about collisions between the active actors - not the ground
	if (s1->GetBody() != _physDemo1->GetBody() && s1->GetBody() != _physDemo2->GetBody())
		return;
	if (s2->GetBody() != _physDemo2->GetBody() && s2->GetBody() != _physDemo2->GetBody())
		return;

	_physDemo1->SetColor(0.0f, MathUtil::RandomFloatInRange(0.f, 1.f), MathUtil::RandomFloatInRange(0.f, 1.f));


	if (_firstSound)
	{
		int result = theSound.PlaySound(_firstSound,1.0f,false,0);

		if (result == SER_SOUND_ALREADY_PLAYING)
		{
			//std::cout << "Sound already playing." << std::endl;
		}
		// Success.
		else
		{
			theActorFactory.BeginActor("particleTest", 0);
				theActorFactory.GetCurrentActor()->SetPosition(hitLocation.X, hitLocation.Y);
			theActorFactory.EndActor();
		}
	}
}

void DemoGame::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
	Vector2 world = MathUtil::ScreenToWorld(screenCoordinates.X, screenCoordinates.Y);
	std::cout << "Clicked at " << world.X << ", " << world.Y << std::endl;
}

bool DemoGame::IsProtectedFromUnloadAll( Renderable* pRenderable )
{
	return ( 
		pRenderable ==  _demo1  ||
		pRenderable ==  _demo2  ||
		pRenderable ==  _demo3  ||
		pRenderable ==  _demo4  ||
		pRenderable ==  _demo5  ||
		pRenderable ==  _demo6  ||
		pRenderable ==  _physGround  ||
		pRenderable ==  _physDemo1  ||
		pRenderable ==  _physDemo2
		);

}
