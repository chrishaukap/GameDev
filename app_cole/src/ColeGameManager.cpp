
#include "ColeGameManager.h"

#include "World.h"
#include "TextRendering.h"
#include "ColeGameScreen.h"

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


ColeGameManager* ColeGameManager::s_ColeGameManager = NULL;
ColeGameManager::ColeGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, ColeGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, ColeGameManager, MoveBackwards);	
   CONSOLE_DECLARECMDMETHOD(W_Down, this, ColeGameManager, W_Down);
   CONSOLE_DECLARECMDMETHOD(A_Down, this, ColeGameManager, A_Down);
   CONSOLE_DECLARECMDMETHOD(D_Down, this, ColeGameManager, D_Down);
   CONSOLE_DECLARECMDMETHOD(W_Up, this, ColeGameManager, W_Up);
   CONSOLE_DECLARECMDMETHOD(A_Up, this, ColeGameManager, A_Up);
   CONSOLE_DECLARECMDMETHOD(D_Up, this, ColeGameManager, D_Up);

	_screens.push_back(new ColeGameScreen());							// 0

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

ColeGameManager& ColeGameManager::getInstance()
{
	if (s_ColeGameManager == NULL)
	{
		s_ColeGameManager = new ColeGameManager();
	}
	return *s_ColeGameManager;
}

void 
ColeGameManager::MoveForwards(const String& input)
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
ColeGameManager::MoveBackwards(const String& input)
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
ColeGameManager::Update(float dt)
{
}
 
void
ColeGameManager::CleanUp()
{
   _screens[_current]->CleanUp();
}

void 
ColeGameManager::Render()
{
}

static bool wIsDown = false;
static bool aIsDown = false;
static bool dIsDown = false;
void ColeGameManager::W_Down(const String& input){wIsDown = true;}
void ColeGameManager::A_Down(const String& input){aIsDown = true;}
void ColeGameManager::D_Down(const String& input){dIsDown = true;}
void ColeGameManager::W_Up(const String& input){wIsDown = false;}
void ColeGameManager::A_Up(const String& input){aIsDown = false;}
void ColeGameManager::D_Up(const String& input){dIsDown = false;}
bool ColeGameManager::isADown() const{return aIsDown;}
bool ColeGameManager::isDDown() const{return dIsDown;}
bool ColeGameManager::isWDown() const{return wIsDown;}