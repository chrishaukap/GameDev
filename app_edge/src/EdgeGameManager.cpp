
#include "EdgeGameManager.h"

#include "World.h"
#include "TextRendering.h"
#include "EdgeGameScreen.h"

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


EdgeGameManager* EdgeGameManager::s_EdgeGameManager = NULL;
EdgeGameManager::EdgeGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, EdgeGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, EdgeGameManager, MoveBackwards);

	_screens.push_back(new EdgeGameScreen());							// 0

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

EdgeGameManager& EdgeGameManager::getInstance()
{
	if (s_EdgeGameManager == NULL)
	{
		s_EdgeGameManager = new EdgeGameManager();
	}
	return *s_EdgeGameManager;
}

void 
EdgeGameManager::MoveForwards(const String& input)
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
EdgeGameManager::MoveBackwards(const String& input)
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
EdgeGameManager::Update(float dt)
{
}
 
void
EdgeGameManager::CleanUp()
{
   _screens[_current]->CleanUp();
}

void 
EdgeGameManager::Render()
{
}
