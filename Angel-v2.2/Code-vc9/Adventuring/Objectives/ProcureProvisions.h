#pragma once

#include "Objectives/Objective.h"
class Hero;
class Hunt: public Objective
{
public:
   Hunt(Hero& hero, float durationOfHunt);
   ~Hunt();
   void applyWork(float dt);

private:
   void checkForMorePrereqs();
   bool objectiveMet() const;
   Hero& m_hero;
   float m_durationOfHunt;
   float m_targetDurationOfHunt;
   int m_killChanceCounter;
};
