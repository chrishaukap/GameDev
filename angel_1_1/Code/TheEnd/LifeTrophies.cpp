#include "stdafx.h"
#include "LifeTrophies.h"
#include "LifeChoicePoints.h"
#include <assert.h>

LifeTrophies* LifeTrophies::s_instance = NULL;

LifeTrophies& LifeTrophies::GetInstance()
{
   if (s_instance == NULL)
   {
	   s_instance = new LifeTrophies();
   }
   return *s_instance;
}
void LifeTrophies::Destroy()
{
	delete s_instance;
	s_instance = NULL;
}

LifeTrophies::LifeTrophies() : 
   m_lifeTrophies(),
   m_trophiesEarned(),
   m_trophiesLost()
{}
LifeTrophies::~LifeTrophies()
{   
	m_lifeTrophies.clear();
   m_trophiesEarned.clear();
   m_trophiesLost.clear();
}


void 
LifeTrophies::processTrophies(AgeGroups::Group agegroup, LIFE_TROPHIES& processedTrophies)
{
   processedTrophies.clear();

   // process upkeep (and potentially lost items first)
	unsigned int numTrophies = (unsigned int)m_lifeTrophies.size();   
	for(unsigned int i=0; i<numTrophies; ++i)
	{
      LifeTrophy* trophy = &(m_lifeTrophies[i]);
      // remove state from last age stage
      trophy->m_earnedThisStage = false;
      trophy->m_lostThisStage = false;

		// process upkeep
      if(trophy->m_earned)
      {
			unsigned int numUpkeeps = (unsigned int)trophy->m_upkeep.size();
		   for(unsigned int j=0; j<numUpkeeps; ++j)
		   {
			   thePointTracker.addUpkeepToLifeChoice( trophy->m_upkeep[j] );		
				if( thePointTracker.getPointsInLifeChoice( trophy->m_upkeep[j].name ) <
					 trophy->m_choiceRequirements[ trophy->m_upkeep[j].name ] )
            {
               trophy->m_earned = false; // they lost their trophy!
               trophy->m_lostThisStage = true;
            }
		   }
         processedTrophies.push_back(*trophy);
      }
	}

   // process unearned items next 
	for(unsigned int i=0; i<numTrophies; ++i)
	{
      LifeTrophy* trophy = &(m_lifeTrophies[i]);

      if( !trophy->m_earned )
      {
         if(requirementsHaveBeenMet( *trophy, agegroup ))
         {
            trophy->m_earned = true;
            trophy->m_earnedThisStage = true;
            processedTrophies.push_back(*trophy);
         }
      }
	}
}

bool
LifeTrophies::requirementsHaveBeenMet( const LifeTrophy& trophy, AgeGroups::Group agegroup ) const
{
   unsigned int numTrophyReqs = (unsigned int)trophy.m_trophyRequirements.size();   
   for(unsigned int i=0; i<numTrophyReqs; ++i)
   {
      unsigned int numTrophies = (unsigned int)m_lifeTrophies.size();   
      for(unsigned int j=0; j<numTrophies; ++j)
         if( 0 == _stricmp( m_lifeTrophies[j].m_name.c_str(), trophy.m_trophyRequirements[i].c_str() ) )
            if( !m_lifeTrophies[j].m_earned )
					return false;   
   }

	LifeTrophy::LIFECHOICE_REQSMAP::const_iterator iter = trophy.m_choiceRequirements.begin();
	while( iter != trophy.m_choiceRequirements.end() )
	{		
      unsigned int points = thePointTracker.getPointsInLifeChoice( iter->first );
      if(points < iter->second )
         return false;
		++iter;
   }

   if( trophy.m_minAgeGroup > agegroup )
      return false;
   if( trophy.m_maxAgeGroup < agegroup )
      return false;

   return true;
}