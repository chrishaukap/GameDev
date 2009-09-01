
#include "JakeGameManager.h"

#include "World.h"
#include "TextRendering.h"
#include "JakeGameScreen.h"

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


JakeGameManager* JakeGameManager::s_JakeGameManager = NULL;
JakeGameManager::JakeGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, JakeGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, JakeGameManager, MoveBackwards);	
   CONSOLE_DECLARECMDMETHOD(W_Down, this, JakeGameManager, W_Down);
   CONSOLE_DECLARECMDMETHOD(A_Down, this, JakeGameManager, A_Down);
   CONSOLE_DECLARECMDMETHOD(D_Down, this, JakeGameManager, D_Down);
   CONSOLE_DECLARECMDMETHOD(W_Up, this, JakeGameManager, W_Up);
   CONSOLE_DECLARECMDMETHOD(A_Up, this, JakeGameManager, A_Up);
   CONSOLE_DECLARECMDMETHOD(D_Up, this, JakeGameManager, D_Up);

	_screens.push_back(new JakeGameScreen());							// 0

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

JakeGameManager& JakeGameManager::getInstance()
{
	if (s_JakeGameManager == NULL)
	{
		s_JakeGameManager = new JakeGameManager();
	}
	return *s_JakeGameManager;
}

void 
JakeGameManager::MoveForwards(const String& input)
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
JakeGameManager::MoveBackwards(const String& input)
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
JakeGameManager::Update(float dt)
{
}
 
void
JakeGameManager::CleanUp()
{
   _screens[_current]->CleanUp();
}

void 
JakeGameManager::Render()
{
}

static bool wIsDown = false;
static bool aIsDown = false;
static bool dIsDown = false;
void 
JakeGameManager::W_Down(const String& input)
{	
	wIsDown = true;
}
void 
JakeGameManager::A_Down(const String& input)
{
	aIsDown = true;
}
void 
JakeGameManager::D_Down(const String& input)
{
	dIsDown = true;
}
void JakeGameManager::W_Up(const String& input){wIsDown = false;}
void JakeGameManager::A_Up(const String& input){aIsDown = false;}
void JakeGameManager::D_Up(const String& input){dIsDown = false;}

bool JakeGameManager::isADown() const{return aIsDown;}
bool JakeGameManager::isDDown() const{return dIsDown;}
bool JakeGameManager::isWDown() const{return wIsDown;}