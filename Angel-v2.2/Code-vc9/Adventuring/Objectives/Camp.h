#pragma once

#include "Objectives/Objective.h"
class Hero;
class Camp : public Objective
{
public:
   Camp(Hero& hero, float durationOfCamping);
   ~Camp();
   void applyWork(float dt);

private:
   bool objectiveMet() const;
   Hero& m_hero;
   float m_durationOfCamping;
   float m_targetDurationOfCamping;
};
