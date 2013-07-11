#pragma once

#include <list>

class Combatant;
class Combat
{
public:
   Combat(std::list<Combatant*> force1,
          std::list<Combatant*> force2);
   virtual ~Combat();

   void Update(float dt);
   bool combatOver() const;

protected:

   void RemoveDeadCombatants();
   void ExecuteRound();
   std::list<Combatant*> m_force1;
   std::list<Combatant*> m_force2;
   float m_combatDuration;
   float m_combatRoundCount;
};