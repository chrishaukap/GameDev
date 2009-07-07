
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
}

void EdgeGameScreen::Start()
{
	m_game = new Game(16,11);
	m_game->start();
	
	#pragma region Demo Housekeeping
	TextActor *fileLoc = new TextActor("ConsoleSmall", "EdgeGameScreen.cpp");
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


void EdgeGameScreen::CleanUp()
{
   m_game->cleanup();
}

void EdgeGameScreen::Render()
{	
}
void EdgeGameScreen::Update(float dt)
{
   m_game->update(dt);
}

