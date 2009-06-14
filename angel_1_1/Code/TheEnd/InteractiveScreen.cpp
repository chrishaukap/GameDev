#include "stdafx.h"

#include "InteractiveScreen.h"
#include "AdvanceButton.h"
#include "LifeChoicePoints.h"

#include "World.h"
#include "TextActor.h"

InteractiveScreen::InteractiveScreen() :
   m_t(NULL),
   m_level(NULL)
{}

void InteractiveScreen::Stop()
{
	thePointTracker.RemoveLifeChoicesFromWorld();
   theAdvanceButton.RemoveFromWorld();
   GameScreen::Stop();
}

void InteractiveScreen::Kill()
{
   thePointTracker.RemoveLifeChoicesFromWorld();
   theAdvanceButton.RemoveFromWorld();
   GameScreen::Kill();	
}
void InteractiveScreen::Start()
{
	// HAUKAP - ability to define levels (including choices allowed, available points) in a file
   thePointTracker.setAvailablePoints(10);
	thePointTracker.AddLifeChoicesToWorld(m_level->m_availableLifeChoices);
	//thePointTracker.ZeroOutPointsInLifeChoices();

   theAdvanceButton.SetSize(3.0f);
   theAdvanceButton.SetColor(0,0,0);
   theAdvanceButton.SetPosition(10,-5);
   theAdvanceButton.AddToWorld();
	
	//Demo housekeeping below this point. 
	#pragma region Demo Housekeeping
   String desc = "Interactive Screen ";
	desc += AgeGroups::m_ageGroupStrings[m_level->m_ageGroup];

	m_t = new TextActor("Console", desc);
	m_t->SetPosition(0, 0);
	m_t->SetAlignment(TXT_Center);
	theWorld.Add(m_t);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "InteractiveScreen.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);

	m_objects.push_back(fileLoc);
	m_objects.push_back(m_t);

	#pragma endregion
}

void InteractiveScreen::Update(float dt)
{
   theAdvanceButton.Update(dt);
}