#include "stdafx.h"
#include "GainLossScreen.h"

#include "World.h"
#include "TextActor.h"
#include "LifeTrophies.h"

GainLossScreen::GainLossScreen(AgeGroups::Group agegroup) : m_gainsLosses(), m_agegroup(agegroup)
{
}

void GainLossScreen::Stop()
{
   GameScreen::Stop();
   unsigned int sz = (unsigned int)m_gainsLosses.size();
   for( unsigned int i=0; i<sz; ++i )
      theWorld.Remove( m_gainsLosses[i] );
}
void GainLossScreen::Start()
{
   LifeTrophies::LIFE_TROPHIES trophies; 
	theLifeTrophies.processTrophies(m_agegroup, trophies);

	float y = 7.0f;
	unsigned int sz = (unsigned int)trophies.size();
	for(unsigned int i=0; i<sz; ++i)
	{
      String description;
      if( trophies[i].m_earnedThisStage )
         description += "EARNED: ";
      else if( trophies[i].m_lostThisStage )
         description += "LOST: ";
      description += trophies[i].m_description;

		TextActor *actor = new TextActor( "ConsoleSmall", description );
      actor->SetPosition(0.0f, y);
      actor->SetSize(15.0f, 1);
      actor->SetAlignment(TXT_Center);
		y -= 1.5f;
      m_gainsLosses.push_back(actor);
      theWorld.Add(actor);
	}

	//TextActors, oddly enough, let you display text!
	TextActor *t = new TextActor("Console", "GainLossScreen");
	t->SetPosition(0, 0);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);

	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	TextActor *fileLoc = new TextActor("ConsoleSmall", "GainLossScreen.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	m_objects.push_back(fileLoc);
	m_objects.push_back(t);
	#pragma endregion
}

