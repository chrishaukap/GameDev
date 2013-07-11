#include "stdafx.h"

#include "Engine/Hero.h"
#include "Objectives/Objective.h"
#include "Objectives/Camp.h"
#include "Objectives/ProcureProvisions.h"
#include "Engine/GameWorld.h"

Hero::Hero() : 
   Combatant("Hero"),
   m_objectives(),
   m_objectiveOverrides(),
   m_currentLocation(NULL),
   m_motivation(100),
   m_fatigue(0),
   m_provisions(7.0f)
{
   m_health = 100; 
}

Hero::~Hero() 
{
   std::list<Objective*>::iterator iter = m_objectives.begin();
   while(iter != m_objectives.end())
   {
      delete(*iter);
      ++iter;
   }
   m_objectives.clear();
   
   iter = m_objectiveOverrides.begin();
   while(iter != m_objectiveOverrides.end())
   {
      delete(*iter);
      ++iter;
   }
   m_objectiveOverrides.clear();
}
const Objective* 
Hero::mainObjective() const
{
   return m_objectives.empty() ? NULL : m_objectives.front();
}
Objective* 
Hero::mainObjective()
{
   return m_objectives.empty() ? NULL : m_objectives.front();
}
const Objective*
Hero::activeObjective() const
{
   if(!m_objectiveOverrides.empty())
      return &m_objectiveOverrides.front()->activeObjective();
   else
      return m_objectives.empty() ? NULL : &m_objectives.front()->activeObjective();
}

void Hero::pushObjective(Objective *obj)
{
   m_objectives.push_back(obj);
}
void Hero::popMainObjective()
{
   delete m_objectives.front();
   m_objectives.pop_front();
}

void Hero::UpdateCombatant(float dt)
{
   DoWork(dt);
}

void Hero::CheckForObjectiveOverrides()
{
   // HAUKAP - decision to rest/camp/hunt should be based off of hero's necessity
   //          with various weights due to laziness/locations/intelligence, etc.
   if(m_provisions <= 1.0f)
      m_objectiveOverrides.push_back(new Hunt(*this, 2.0f));
   if(m_fatigue >= 400)
      m_objectiveOverrides.push_back(new Camp(*this, 3.0f));
}
void Hero::WorkOnObjectives(float dt)
{
   if(!m_objectives.empty() && !mainObjective()->isComplete())   
      mainObjective()->doWork(dt);

   if(!m_objectives.empty() && mainObjective()->isComplete())
      popMainObjective();
}
void Hero::WorkOnOverrides(float dt)
{
   if(!m_objectiveOverrides.empty() && !(*m_objectiveOverrides.begin())->isComplete())   
      (*m_objectiveOverrides.begin())->doWork(dt);

   if(!m_objectiveOverrides.empty() && (*m_objectiveOverrides.begin())->isComplete())
   {
      delete m_objectiveOverrides.front();
      m_objectiveOverrides.pop_front();
   }
}
void Hero::DoWork(float dt)
{
   if(m_objectiveOverrides.empty())
      CheckForObjectiveOverrides();
   
   if(!m_objectiveOverrides.empty())
      WorkOnOverrides(dt);

   if(m_objectiveOverrides.empty())
      WorkOnObjectives(dt);
}


void Hero::setCurrentLocation(Location& location)
{
   m_currentLocation = &location;
}
Location* Hero::getCurrentLocation() const
{
   return m_currentLocation;
}

static const int BaseArcaneExperienceValue = 100; // base xp unit 
void Hero::applyArcaneExperience(float time, float effectiveness)
{
   m_arcaneExperience += (int)(BaseArcaneExperienceValue * time * effectiveness);

   ArcaneLevelThresholdMap thresholdMap = theGameWorld.arcaneLevelThresholds();
   if(m_arcaneExperience >= thresholdMap[m_arcaneLevel])
      levelUpArcane();
}

static const float FatigueRegenRate = 400; 
void Hero::applyRest(float timeSpentResting, float effectivenessOfRestArea)
{
   m_fatigue -= FatigueRegenRate * timeSpentResting * effectivenessOfRestArea;
   if(m_fatigue < 0)
      m_fatigue = 0;
}

static const int ProvisionsUsageRate = 1;
void Hero::consumeProvisions( float timeSpentConsuming )
{
   m_provisions -= float(ProvisionsUsageRate) * timeSpentConsuming;
   if(m_provisions < 0)
      m_provisions = 0;
}
void Hero::addProvisions( float provisionsToAdd )
{
   m_provisions += provisionsToAdd;
}

static const int MaxFatigueThreshold = 400;
bool Hero::wantsToWork() const
{
   return m_fatigue <= MaxFatigueThreshold;
}

void Hero::applyFatigue( float fatigueToAdd)
{
   m_fatigue += fatigueToAdd;
}

bool Hero::shouldEngage( const class Monster& monster)
{
   if(inCombat())
      return false;
   return true;

}
bool Hero::shouldEngage( const class Hero& hero)
{
   if(m_combat != NULL)
      return false;
   // maybe we'll do so if we're evil?
   return false;
}

void Hero::setState(Hero::HeroState state)
{
   m_state = state;
}
Hero::HeroState Hero::state() const
{
   return m_state;
}

std::string Hero::stateToString() const
{
   switch(m_state)
   {
   case Camping: return "Camping";
   case Resting: return "Resting";
   case MartialTraining: return "MartialTraining";
   case ArcheryTraining: return "ArcheryTraining";
   case ArcaneTraining: return "ArcaneTraining";
   case Researching: return "Researching";
   case Hunting: return "Hunting";
   case Travel: return "Travel";
   }
}