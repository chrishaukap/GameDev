#include "stdafx.h"

#include "Engine/Combatant.h"

Combatant::Combatant(std::string name) : 
   m_combat(NULL),
   m_name(name),
   m_health(20),
   m_intelligence(10),
   m_motivation(100),
   m_combatFatigue(0),
   m_arcaneLevel(1),
   m_arcaneExperience(0),
   m_spells()
{      
   static int idCounter = 0;
   char buf[99999];
   sprintf(buf, "%d", idCounter++);
   m_name += buf;   
}

Combatant::~Combatant() 
{
}

void Combatant::levelUpArcane()
{
   m_arcaneLevel++;
   m_intelligence += 5;
}

void Combatant::applyDamage( int damage)
{
   m_health -= damage;
}
void Combatant::applyCombatFatigue( float fatigueToAdd)
{
   m_combatFatigue += fatigueToAdd;
}