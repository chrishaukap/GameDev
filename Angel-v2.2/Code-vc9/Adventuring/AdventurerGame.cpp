#include "stdafx.h"
#include "AdventurerGame.h"

#include "Engine/GameWorld.h"
#include "screens/WorldScreen.h"
#include "screens/DetailScreen.h"

AdventurerGame* AdventurerGame::s_AdventurerGame = NULL;
AdventurerGame::~AdventurerGame()
{
   delete s_AdventurerGame;
}
AdventurerGame::AdventurerGame() :
   m_worldScreen(NULL),
   m_detailScreen(NULL),
   m_screen(Invalid)
{	
	//subscribe to messages
	theSwitchboard.SubscribeTo(this, "WorldView");
	theSwitchboard.SubscribeTo(this, "DetailView");
}
void AdventurerGame::Init(unsigned int screenWidth, unsigned int screenHeight)
{
   m_worldScreen = new WorldScreen(screenWidth, screenHeight);							
   m_detailScreen = new DetailScreen(screenWidth, screenHeight);

   m_worldScreen->ShouldUpdate(false);
   m_detailScreen->ShouldUpdate(false);
   
   gotoWorldView();   
}

AdventurerGame& AdventurerGame::GetInstance()
{
	if (s_AdventurerGame == NULL)
	{
		s_AdventurerGame = new AdventurerGame();
	}
	return *s_AdventurerGame;
}

void AdventurerGame::ReceiveMessage(Message* message)
{
	if (message->GetMessageName() == "DetailView")
	{
		gotoDetailView();
	}
   else if(message->GetMessageName() == "WorldView")
   {
      gotoWorldView();
   }
}
void AdventurerGame::gotoWorldView()
{
   if(m_screen != WorldView)
   {
      m_detailScreen->Pause();
      theWorld.Remove(m_detailScreen);

      if(! m_worldScreen->isStarted())
         m_worldScreen->Start();
      else
         m_worldScreen->Resume();

      m_screen = WorldView;
      theWorld.Add(m_worldScreen);
   }
}

void AdventurerGame::gotoDetailView()
{
   if(m_screen != DetailView)
   {
      m_worldScreen->Pause();
      theWorld.Remove(m_worldScreen);

      if(! m_detailScreen->isStarted())
         m_detailScreen->Start();
      else
         m_detailScreen->Resume();

      m_screen = DetailView;
      theWorld.Add(m_detailScreen);
   }
}


void AdventurerGame::Update(float dt)
{
   theGameWorld.step(dt);
   if(m_screen == WorldView)
      m_worldScreen->Update(dt);
   else if(m_screen == DetailView)
      m_detailScreen->Update(dt);
}
void AdventurerGame::Render()
{
}