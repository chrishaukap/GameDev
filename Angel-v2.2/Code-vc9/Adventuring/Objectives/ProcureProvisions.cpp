#include "stdafx.h"

#include "Objectives/ProcureProvisions.h"
#include "Engine/Hero.h"

Hunt::Hunt(Hero& hero, float durationOfHunt) :
   m_hero(hero),
   m_durationOfHunt(0),
   m_targetDurationOfHunt(durationOfHunt),
   m_killChanceCounter(0)
{
   m_name = "Hunting... ";
}
Hunt::~Hunt()
{}

void Hunt::applyWork(float dt)
{
   m_hero.setState( Hero::Hunting );

   m_durationOfHunt += dt;
   int accumSeconds = (int)m_durationOfHunt;
   if( accumSeconds - m_killChanceCounter >= 1)
   {
      // another chance to make a kill
      float roll = MathUtil::RandomFloat(100.0f);
      if(roll >= 50.f)
         m_hero.addProvisions(2);
      ++m_killChanceCounter;
   }
}
bool Hunt::objectiveMet() const
{
   return m_durationOfHunt >= m_targetDurationOfHunt;
}


void Hunt::checkForMorePrereqs() 
{}
