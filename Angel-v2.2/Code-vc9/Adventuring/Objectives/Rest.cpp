#include "stdafx.h"

#include "Objectives/Rest.h"
#include "Engine/Hero.h"

Rest::Rest(Hero& hero, float durationOfRest) :
   m_hero(hero),
   m_durationOfRest(0),
   m_targetDurationOfRest(durationOfRest)
{
   m_name = "Resting...";
}
Rest::~Rest()
{}

void Rest::applyWork(float dt)
{
   m_hero.setState( Hero::Resting );

   m_durationOfRest += dt;
   m_hero.applyRest(dt, 1.0f /*HAUKAP - effectiveness of training based off of location*/);
}
bool Rest::objectiveMet() const
{
   return m_durationOfRest >= m_targetDurationOfRest;
}

