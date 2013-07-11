#pragma once

#include "Objectives/Objective.h"
class Hero;
enum VocationType
{
   InvalidVocation = 0,
   Arcane,
   Melee,
   Archery
};
class Train : public Objective
{
public:
   Train(Hero& hero, VocationType vocation, float durationOfTraining);
   ~Train();
   void applyWork(float dt);

private:
   bool objectiveMet() const;
   Hero& m_hero;
   float m_durationOfTraining;
   float m_targetDurationOfTraining;
   VocationType m_vocation;
};
