#include "stdafx.h"
#include "TetrisGameScreen.h"
#include "Game.h"
#include "TetrisRenderer_OpenGL.h"
#include "DrawUtil.h"
#include "MathUtil.h"

#include "World.h"
using namespace CDH;
using namespace Tetris;

TetrisGameScreen::TetrisGameScreen() :
	t(NULL),
	m_game(NULL),
	m_tetrisRenderer(NULL)
{   
	CONSOLE_DECLARECMDMETHOD(Rotate, this, TetrisGameScreen, Rotate);
	CONSOLE_DECLARECMDMETHOD(MoveBlockLeft, this, TetrisGameScreen, MoveBlockLeft);
	CONSOLE_DECLARECMDMETHOD(MoveBlockRight, this, TetrisGameScreen, MoveBlockRight);
	CONSOLE_DECLARECMDMETHOD(MoveBlockDown, this, TetrisGameScreen, MoveBlockDown);
}

void TetrisGameScreen::Start()
{
	m_game = new Game(16,11);
	m_game->start();
	m_tetrisRenderer = new TetrisRenderer_OpenGL(*m_game);
	
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
	#pragma endregion
}

void TetrisGameScreen::Stop()
{	
	m_game->stop();
	delete m_game;
	m_game = NULL;

	delete m_tetrisRenderer;
	m_tetrisRenderer = NULL;
}


void TetrisGameScreen::Render()
{	
	m_tetrisRenderer->render();
}
void TetrisGameScreen::Update(float dt)
{
   m_game->update(dt);
}

void 
TetrisGameScreen::Rotate(const String& input)
{
}
void 
TetrisGameScreen::MoveBlockLeft(const String& input)
{
   m_game->moveActiveBlockLeft();
}
void 
TetrisGameScreen::MoveBlockRight(const String& input)
{
   m_game->moveActiveBlockRight();
}
void 
TetrisGameScreen::MoveBlockDown(const String& input)
{
   m_game->moveActiveBlockDown();
}