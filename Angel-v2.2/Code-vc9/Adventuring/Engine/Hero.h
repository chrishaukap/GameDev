#pragma once

#include "Combatant.h"

class Objective;
class Hero: public Combatant
{
public:
   Hero();
   virtual ~Hero();

   enum HeroState
   {
      Camping, Resting, MartialTraining, ArcheryTraining, ArcaneTraining, 
      Researching, Hunting, Travel
   };

   // takes ownership of the objective
   void pushObjective(Objective *obj);

   // returns NULL if no main objective
   const Objective* mainObjective() const;
   const Objective* activeObjective() const;

   void setCurrentLocation(class Location& location);
   Location* getCurrentLocation() const;

   float fatigue() const {return m_fatigue;}
   float provisions() const {return m_provisions;}

   void applyArcaneExperience(float timeSpentResearching, 
                              float effectivenessOfResearch);  
   
   void applyRest(float timeSpentResting, float effectivenessOfRestArea);
   void applyFatigue( float fatigueToAdd);
   bool wantsToWork() const;
   void consumeProvisions( float timeSpentConsuming );
   void addProvisions( float provisionsToAdd );

   bool shouldEngage( const class Monster& monster);
   bool shouldEngage( const class Hero& hero);

   void setState(HeroState state);
   HeroState state() const;
   std::string stateToString() const;
protected: 
   void UpdateCombatant(float dt);
private:
   std::list<Objective*> m_objectives;
   std::list<Objective*> m_objectiveOverrides; // distractions from the main quest.  (resting/hunting/camping/etc.)
   
   Objective* mainObjective();
   
   // If NULL, then not at a specific location
   class Location* m_currentLocation; 
   void popMainObjective();
   void DoWork(float dt);
   void WorkOnObjectives(float dt);
   void WorkOnOverrides(float dt);

   void CheckForObjectiveOverrides();

   // stats
   int m_motivation;
   float m_fatigue;

   // supplies
   float m_provisions;
   HeroState m_state;
};