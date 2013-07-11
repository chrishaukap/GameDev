#include "stdafx.h"
#include "WorldScreen.h"
#include "Engine/ScrollingActor.h"
#include "Engine/GameWorld.h"
#include "Engine/Hero.h"
#include "Engine/Monster.h"
#include "Locations/Location.h"
#include "Objectives/Objective.h"
#include <map>
#include <string>
       
static std::map<std::string, TextActor*> s_debugInfoMap;
WorldScreen::WorldScreen(unsigned int screenWidth, unsigned int screenHeight)
{
   m_screenWidth = screenWidth;
   m_screenHeight = screenHeight;
}
WorldScreen::~WorldScreen()
{
   std::map<std::string, TextActor*>::iterator iter = s_debugInfoMap.begin();
   while( iter != s_debugInfoMap.end())
   {
      delete iter->second;
      ++iter;
   }
   s_debugInfoMap.clear();
}

static Vector3 s_CameraPosition;
void WorldScreen::doPause()
{
   s_CameraPosition = theCamera.GetPosition3();
}
void WorldScreen::doResume()
{
   theCamera.SetPosition(s_CameraPosition);
}
void WorldScreen::doStop()
{
   delete m_backgroundMap;
}
void WorldScreen::doStart()
{
   // initialize background actor
   {
	   m_backgroundMap = new BackgroundActor();
      
	   Vector2 maxVert = theCamera.GetWorldMaxVertex();
	   Vector2 minVert = theCamera.GetWorldMinVertex();
	   Vector2 dimensions = maxVert - minVert;

      m_backgroundMap->SetSize(dimensions.X, dimensions.Y);
	   m_backgroundMap->SetColor(1,1,1,1); //(white and opaque so the texture comes through fully)
	   m_backgroundMap->ClearSpriteInfo();
	   m_backgroundMap->SetSprite("Resources/Images/worldmap.png", 0, GL_REPEAT ); //Loads any image supported by DevIL (see docs)
      
      AddRenderable(m_backgroundMap, 0);
   }

   // initialize hero actor
   {
      Hero* hero = const_cast<Hero*>(&theGameWorld.hero());
      TextActor* heroDebugText = new TextActor();
      heroDebugText->SetColor(0,0,0);
	   heroDebugText->SetAlignment(TXT_Left);
      heroDebugText->SetSize(1);

      AddRenderable(hero, 2);
      AddRenderable(heroDebugText, 10);

      s_debugInfoMap[hero->name()] = heroDebugText;
   }

   // initialize Location actors
   const Locations& locations = theGameWorld.locations();
   Locations::const_iterator iter = locations.begin();
   while(iter != locations.end())
   {   
      Location* location = const_cast<Location*>(*iter);
      TextActor *debugText = new TextActor();
         
      debugText->SetColor(0,0,0);
      debugText->SetAlignment(TXT_Left);
      debugText->SetSize(1);
      debugText->SetPosition(location->GetPosition().X, location->GetPosition().Y);
      s_debugInfoMap[(*iter)->name().c_str()] = debugText;

      AddRenderable(location, 1);
      AddRenderable(debugText, 10);

      ++iter;
   }

   // initialize monster actors
   const Monsters& monsters = theGameWorld.monsters();
   Monsters::const_iterator iter2 = monsters.begin();
   while(iter2 != monsters.end())
   {   
      Monster* monster = const_cast<Monster*>(*iter2);
      TextActor *debugText = new TextActor();
         
      debugText->SetColor(0,0,0);
      debugText->SetAlignment(TXT_Left);
      debugText->SetSize(1);
      debugText->SetPosition(monster->GetPosition().X, monster->GetPosition().Y);

      AddRenderable(monster, 2);
      AddRenderable(debugText, 10);

      ++iter2;
   }

   theGameWorld.DebugInit();
}

void WorldScreen::UpdateDebug()
{   
   const Hero& hero = theGameWorld.hero();

   const Objective* mainObjective = hero.mainObjective();
   const Objective* activeObjective = hero.activeObjective();
   char buf[512];
   sprintf(buf, 
           "MainQuest: %s\nActiveQuest: %s\nHealth: %d\nIntelligence: %d\n" \
           "ArcaneExperience: %d\nFatigue: %f\nProvisions: %f\nCombat: %s\n", 
           mainObjective == NULL ? "" : mainObjective->name().c_str(),
           activeObjective == NULL ? "" : activeObjective->name().c_str(),
           hero.health(),
           hero.intelligence(), 
           hero.arcaneExperience(),
           hero.fatigue(),
           hero.provisions(),
           hero.inCombat() ? "true" : "false");
   std::string displaystring(buf);;

   s_debugInfoMap[hero.name()]->SetDisplayString(displaystring);
   s_debugInfoMap[hero.name()]->SetPosition(hero.GetPosition().X, hero.GetPosition().Y);

   const Locations& locations = theGameWorld.locations();
   Locations::const_iterator iter = locations.begin();
   while(iter != locations.end())
   {   
      Location* location = const_cast<Location*>(*iter);
      sprintf(buf, "%s\nf:%s", 
              location->GetName().c_str(),
              location->Functional ? "true" : "false");
      
      s_debugInfoMap[location->name().c_str()]->SetDisplayString(std::string(buf));
      ++iter;
   }
}

void WorldScreen::doUpdate(float dt)
{
   UpdateDebug();

   const Hero& hero = theGameWorld.hero();
   Vector3 camPos = theCamera.GetPosition3();
   const float scrollDelta = 0.1f;
   camPos.X = hero.GetPosition().X;
   camPos.Y = hero.GetPosition().Y;

#if 0
   if (/*(theController.IsConnected() && theController.IsYButtonDown()) || */theInput.IsKeyDown('w'))
	{
      camPos.Y += scrollDelta;
   }
	if (theInput.IsKeyDown('a'))
	{
      camPos.X -= scrollDelta;
	}
	if (theInput.IsKeyDown('s'))
	{
      camPos.Y -= scrollDelta;
	}
	if (theInput.IsKeyDown('d'))
	{
      camPos.X += scrollDelta;
	}
#endif
   
   const int zoomTimer = 4;
   static int zoomInTimer = zoomTimer;
   static int zoomOutTimer = zoomTimer;
   const float zoomScale = 1.1f;
	if (theInput.IsKeyDown('z'))
	{
      if( ++zoomInTimer >= zoomTimer )
      {
         zoomInTimer = 0;
         camPos.Z += zoomScale;
      }
	}
	if (theInput.IsKeyDown('x'))
	{
      if( ++zoomOutTimer >= zoomTimer )
      {
         zoomOutTimer = 0;
         camPos.Z -= zoomScale;
      }
	}
   theCamera.SetPosition(camPos);
}
