#include "stdafx.h"
#include "GameScreen.h"

GameScreen::GameScreen() : m_isStarted(false), m_isPaused(false) {}

void GameScreen::Start()
{
   doStart();
   m_isStarted = true;
   UnsuspendGameObjects();
}

void GameScreen::Stop()
{
   m_isStarted = false;
   doStop();
   SuspendGameObjects();
	m_objectLayerMap.clear();
}

void GameScreen::Pause() 
{   
   SuspendGameObjects();

   doPause();
   m_isPaused = true;
}
void GameScreen::Resume() 
{
   doResume();
   UnsuspendGameObjects();
   m_isPaused = false;
}

void GameScreen::AddRenderable(Renderable* renderable, int renderLayer)
{
   renderable->ShouldUpdate(false);
   m_objectLayerMap[renderable] = renderLayer;
}
void GameScreen::ClearRenderablesInScreen()
{
   m_objectLayerMap.clear();
}
void GameScreen::UnsuspendGameObjects() 
{   
	std::map<Renderable*, int>::iterator iter = m_objectLayerMap.begin();
	while(iter != m_objectLayerMap.end())
	{
      theWorld.Add( (*iter).first, (*iter).second );
		++iter;
	}
}
void GameScreen::SuspendGameObjects() 
{
	std::map<Renderable*, int>::iterator iter = m_objectLayerMap.begin();
	while(iter != m_objectLayerMap.end())
	{
      theWorld.Remove( (*iter).first );
		++iter;
	}
}

void GameScreen::Update(float dt) 
{
   // update any derived classes first
   doUpdate(dt);
}
void GameScreen::doUpdate(float dt)
{
   // no op -- to be overriden in derived class
}
void GameScreen::Render() 
{}