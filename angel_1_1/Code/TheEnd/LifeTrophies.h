#pragma once

#include "AgeGroups.h"
#include "utils.h"

#include <vector>
#include <map>

#define theLifeTrophies LifeTrophies::GetInstance()

struct LifeTrophy
{
	LifeTrophy() : 
		m_name(), m_description(), 
		m_minAgeGroup(AgeGroups::Ages0to5), m_maxAgeGroup(AgeGroups::Ages85to90),
		m_choiceRequirements(), m_upkeep(), m_trophyRequirements(), 
      m_earned(false), m_earnedThisStage(false), m_lostThisStage(false)
	{}
	~LifeTrophy(){}
	String m_name;
	String m_description;
	AgeGroups::Group m_minAgeGroup;
	AgeGroups::Group m_maxAgeGroup;

	typedef std::map<String, unsigned int> LIFECHOICE_REQSMAP;
   LIFECHOICE_REQSMAP m_choiceRequirements;
	std::vector<NameValuePair> m_upkeep;
	std::vector<String> m_trophyRequirements;
   bool m_earned;
   bool m_earnedThisStage;
   bool m_lostThisStage;
};

class LifeTrophies
{
public:
   typedef std::vector<LifeTrophy> LIFE_TROPHIES;
   static LifeTrophies& GetInstance();
	static void Destroy();

   LifeTrophies(); 
   ~LifeTrophies();
	void pushBack(LifeTrophy& trophy) { m_lifeTrophies.push_back(trophy); }

	// does processing on each trophy.  should be called once for each Gain/Loss screen
	void processTrophies(AgeGroups::Group agegroup, LIFE_TROPHIES& processedTrophies);

private:
   bool requirementsHaveBeenMet( const LifeTrophy& trophy, AgeGroups::Group agegroup ) const;

	static LifeTrophies* s_instance;
	LIFE_TROPHIES m_lifeTrophies;
   LIFE_TROPHIES m_trophiesEarned;
   LIFE_TROPHIES m_trophiesLost;
};


