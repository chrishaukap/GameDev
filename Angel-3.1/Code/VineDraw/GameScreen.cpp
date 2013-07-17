#include "StdAfx.h"
#include "GameScreen.h"
#include "Infrastructure/World.h"

void GameScreen::Start()
{}
void GameScreen::Stop()
{
	std::vector<Renderable*>::iterator it = m_objects.begin();
	while(m_objects.end() != it)
	{
		theWorld.Remove(*it);
      delete *it;
		it++;
	}
   m_objects.clear();
}