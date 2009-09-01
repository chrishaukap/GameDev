#include "ColeGameScreen.h"
#include "Game.h"
#include "DrawUtil.h"
#include "MathUtil.h"
#include "World.h"

#include "cole.h" // debug drawing...

using namespace CDH;
using namespace Cole;

ColeGameScreen::ColeGameScreen() :
	m_game(NULL)
{   
}

static TextActor *gDebugTextActor = NULL;
void ColeGameScreen::Start()
{
	m_game = new Game(16,11);
	m_game->start();
	
	#pragma region Demo Housekeeping
	gDebugTextActor = new TextActor("ConsoleSmall", "ColeGameScreen.cpp");
	gDebugTextActor->SetPosition(MathUtil::ScreenToWorld(0, 10));
	gDebugTextActor->SetColor(.3f, .3f, .3f);
	theWorld.Add(gDebugTextActor);
	_objects.push_back(gDebugTextActor);
	#pragma endregion
}

void ColeGameScreen::Stop()
{	
	m_game->stop();
	delete m_game;
	m_game = NULL;
}


void ColeGameScreen::CleanUp()
{
   m_game->cleanup();
}

void ColeGameScreen::Render()
{	
}

void ColeGameScreen::UpdateLinearVelocity()
{
	Vector2 linearVelocity;
	m_game->cole()->GetLinearVelocity(linearVelocity);

	char displaystring[255];
	sprintf(displaystring, "LinearVelocity: %f, %f", linearVelocity.X, linearVelocity.Y);
	
	gDebugTextActor->SetDisplayString(displaystring);
}
void ColeGameScreen::Update(float dt)
{
   m_game->update(dt);
	UpdateLinearVelocity();
}

