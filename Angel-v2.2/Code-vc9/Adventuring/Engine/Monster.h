#pragma once

#include "Combatant.h"
class Monster: public Combatant
{
public:
   Monster();
   virtual ~Monster();

   bool shouldEngage(const class Hero& hero);
   
protected:
   void UpdateCombatant(float dt);
private:
   Vector2 m_direction;
};