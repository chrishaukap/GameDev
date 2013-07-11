#pragma once

#include "Objectives/Objective.h"

class BecomePowerfulWizard: public Objective
{
public:
   BecomePowerfulWizard(class Hero& hero);
   ~BecomePowerfulWizard();
   void applyWork(float dt);
private:
   
   bool objectiveMet() const;
   class Hero& m_hero;};