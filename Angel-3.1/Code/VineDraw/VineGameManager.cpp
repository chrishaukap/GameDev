#include "StdAfx.h"
#include "VineGameManager.h"
#include "Infrastructure/TextRendering.h"
#include "Actors/TextActor.h"
#include "Infrastructure/World.h"
#include "VineScreen.h"

VineGameManager::VineGameManager()
{
	// HAUKAP - what did this do again?: CONSOLE_DECLARECMDMETHOD(Restart, this, VineGameManager, Restart);

   //unsigned int width = 0, height = 0;
   //HAUKAP theWorld.getWindowDimensions(width,height);
	m_screens.push_back(new VineScreen(1200, 800)); 

   theWorld.Add(m_screens[ScreenTypeVine]);
   m_screens[ScreenTypeVine]->Start();
	m_currentScreen = ScreenTypeVine;
}
VineGameManager* VineGameManager::s_VineGameManager = NULL;
VineGameManager& VineGameManager::getInstance()
{
	if (s_VineGameManager == NULL)
	{
		s_VineGameManager = new VineGameManager();
	}
	return *s_VineGameManager;
}

void VineGameManager::MoveForwards(const String& input)
{
   unsigned int numScreens = (unsigned int)m_screens.size();
	if ((m_currentScreen >= 0) && (m_currentScreen < (numScreens-1)))
	{
		m_screens[m_currentScreen]->Stop();
		theWorld.Remove(m_screens[m_currentScreen]);

      ++m_currentScreen;
		m_screens[ m_currentScreen ]->Start();
		theWorld.Add(m_screens[m_currentScreen]);
	}
}

void VineGameManager::Restart(const String& input)
{
	assert(0);
	m_screens[m_currentScreen]->Stop();
   m_screens[ScreenTypeVine]->Start();
	m_currentScreen = ScreenTypeVine;
}

void VineGameManager::Update(float dt)
{
   Sleep(30);
}
void VineGameManager::Render()
{
}