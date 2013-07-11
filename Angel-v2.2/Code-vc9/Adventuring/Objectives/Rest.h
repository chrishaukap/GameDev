#pragma once

#include "Objectives/Objective.h"
class Hero;
class Rest : public Objective
{
public:
   Rest(Hero& hero, float durationOfRest);
   ~Rest();
   void applyWork(float dt);

private:
   bool objectiveMet() const;
   Hero& m_hero;
   float m_durationOfRest;
   float m_targetDurationOfRest;
};
