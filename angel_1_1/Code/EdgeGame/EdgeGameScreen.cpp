#include "stdafx.h"
#include "EdgeGameScreen.h"
#include "Game.h"
#include "DrawUtil.h"
#include "MathUtil.h"

#include "World.h"
using namespace CDH;
using namespace Edge;

EdgeGameScreen::EdgeGameScreen() :
	m_game(NULL)
{   
	CONSOLE_DECLARECMDMETHOD(Rotate, this, EdgeGameScreen, Rotate);
	CONSOLE_DECLARECMDMETHOD(MoveBlockLeft, this, EdgeGameScreen, MoveBlockLeft);
	CONSOLE_DECLARECMDMETHOD(MoveBlockRight, this, EdgeGameScreen, MoveBlockRight);
	CONSOLE_DECLARECMDMETHOD(MoveBlockDown, this, EdgeGameScreen, MoveBlockDown);
}

void EdgeGameScreen::Start()
{
	m_game = new Game(16,11);
	m_game->start();
	
	#pragma region Demo Housekeeping
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenSimpleActor.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	#pragma endregion
}

void EdgeGameScreen::Stop()
{	
	m_game->stop();
	delete m_game;
	m_game = NULL;
}


void EdgeGameScreen::Render()
{	
}
void EdgeGameScreen::Update(float dt)
{
   m_game->update(dt);
}

void 
EdgeGameScreen::Rotate(const String& input)
{
}
void 
EdgeGameScreen::MoveBlockLeft(const String& input)
{
}
void 
EdgeGameScreen::MoveBlockRight(const String& input)
{
}
void 
EdgeGameScreen::MoveBlockDown(const String& input)
{
}