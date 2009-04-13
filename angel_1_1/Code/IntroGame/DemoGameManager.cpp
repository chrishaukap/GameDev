#include "StdAfx.h"
#include "DemoGameManager.h"

#include "World.h"
#include "TextRendering.h"

#include "DemoScreenStart.h"
#include "DemoScreenInstructions.h"
#include "DemoScreenSimpleActor.h"
#include "DemoScreenMovingActor.h"
#include "DemoScreenPhysicsActor.h"
#include "DemoScreenCollisions.h"
#include "DemoScreenDefFile.h"
#include "DemoScreenLevelFile.h"
#include "DemoScreenCollisionLevelFile.h"
#include "DemoScreenConsole.h"
#include "DemoScreenLogs.h"
#include "DemoScreenByeBye.h"
#include "DemoScreenRenderLayers.h"
#include "DemoScreenBindingInstructions.h"
#include "DemoScreenLayeredCollisionLevelFile.h"
#include "DemoScreenParticleActors.h"

DemoScreen::DemoScreen() {}

void DemoScreen::Start(){}

void DemoScreen::Stop()
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

void DemoScreen::Update(float dt) {}
void DemoScreen::Render() {}



DemoGameManager* DemoGameManager::s_DemoGameManager = NULL;

DemoGameManager::DemoGameManager()
{
	CONSOLE_DECLARECMDMETHOD(MoveForwards, this, DemoGameManager, MoveForwards);
	CONSOLE_DECLARECMDMETHOD(MoveBackwards, this, DemoGameManager, MoveBackwards);
   CONSOLE_DECLARECMDMETHOD(AddActor, this, DemoGameManager, AddActor);
   CONSOLE_DECLARECMDMETHOD(RemoveActor, this, DemoGameManager, RemoveActor);


	_screens.push_back(new DemoScreenStart());							// 0
	_screens.push_back(new DemoScreenInstructions());					// 1
	_screens.push_back(new DemoScreenSimpleActor());					// 2
	_screens.push_back(new DemoScreenRenderLayers());					// 3
	_screens.push_back(new DemoScreenMovingActor());					// 4
	_screens.push_back(new DemoScreenDefFile());						// 5
	_screens.push_back(new DemoScreenLevelFile());						// 6
	_screens.push_back(new DemoScreenBindingInstructions());			// 7
	_screens.push_back(new DemoScreenParticleActors());					// 8
	_screens.push_back(new DemoScreenPhysicsActor());					// 9
	_screens.push_back(new DemoScreenCollisions());						//10
	_screens.push_back(new DemoScreenCollisionLevelFile());				//11
	_screens.push_back(new DemoScreenLayeredCollisionLevelFile());		//12
	_screens.push_back(new DemoScreenConsole());						//13
	_screens.push_back(new DemoScreenLogs() );							//14
	_screens.push_back(new DemoScreenByeBye());							//15

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

DemoGameManager& DemoGameManager::getInstance()
{
	if (s_DemoGameManager == NULL)
	{
		s_DemoGameManager = new DemoGameManager();
	}
	return *s_DemoGameManager;
}

static Actor* g_actor = NULL;
void DemoGameManager::AddActor(const String& input)
{
   if(g_actor == NULL)
   {
      g_actor = new Actor;
      g_actor->SetColor(1,0,0);
      g_actor->SetPosition(1,1);
      g_actor->SetSize(9.0f);
      theWorld.Add(g_actor);
   }
}
void DemoGameManager::RemoveActor(const String& input)
{
   theWorld.Remove(g_actor);
   delete g_actor;
   g_actor = NULL;
}

void DemoGameManager::MoveForwards(const String& input)
{
	if ((_current >= 0) && (_current < (int)_screens.size() - 1))
	{
		_screens[_current]->Stop();
		theWorld.Remove(_screens[_current]);
		_screens[++_current]->Start();
		theWorld.Add(_screens[_current]);
	}
}

void DemoGameManager::MoveBackwards(const String& input)
{
	if (_current > 0)
	{
		_screens[_current]->Stop();
		theWorld.Remove(_screens[_current]);
		_screens[--_current]->Start();
		theWorld.Add(_screens[_current]);
	}
}

void DemoGameManager::Render()
{

	glColor3f(0.5f, 0.5f, 0.5f);
	char* infoString = "";
	int xOffset = 0;
	if (_current == 0)
	{
		infoString = "[A]: Next ";
		xOffset = 925;
	}
	else if (_current == _screens.size() - 1)
	{
		infoString = "[Back]: Previous";
		xOffset = 870;
	}
	else
	{
		infoString = "[A]: Next [Back]: Previous";
		xOffset = 785;
	}
	DrawGameText(infoString, "ConsoleSmall", xOffset, 763);
}
