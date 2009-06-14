#include "StdAfx.h"
#include "TetrisGameManager.h"

#include "World.h"
#include "TextRendering.h"
#include "TetrisGameScreen.h"


GameScreen::GameScreen() {}

void GameScreen::Start(){}

void GameScreen::Stop()
{
	std::vector<Renderable*>::iterator it = _objects.begin();
	while(_objects.end() != it)
	{
		theWorld.Remove(*it);
		delete *it;
		it++;
	}
	_objects.clear();
}

void GameScreen::Update(float dt) {}
void GameScreen::Render() {}


TetrisGameManager* TetrisGameManager::s_TetrisGameManager = NULL;
TetrisGameManager::TetrisGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, TetrisGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, TetrisGameManager, MoveBackwards);

	_screens.push_back(new TetrisGameScreen());							// 0

	unsigned int startingIndex = 0;
	if (_screens.size() > startingIndex)
	{
		theWorld.Add(_screens[startingIndex]);
		_screens[startingIndex]->Start();
		_current = startingIndex;
	}
	else
	{
		_current = -1;
	}
}

TetrisGameManager& TetrisGameManager::getInstance()
{
	if (s_TetrisGameManager == NULL)
	{
		s_TetrisGameManager = new TetrisGameManager();
	}
	return *s_TetrisGameManager;
}

void 
TetrisGameManager::MoveForwards(const String& input)
{
	if ((_current >= 0) && (_current < (int)_screens.size() - 1))
	{
		_screens[_current]->Stop();
		theWorld.Remove(_screens[_current]);
		_screens[++_current]->Start();
		theWorld.Add(_screens[_current]);
	}
}

void 
TetrisGameManager::MoveBackwards(const String& input)
{
	if (_current > 0)
	{
		_screens[_current]->Stop();
		theWorld.Remove(_screens[_current]);
		_screens[--_current]->Start();
		theWorld.Add(_screens[_current]);
	}
}


void 
TetrisGameManager::Render()
{
}
