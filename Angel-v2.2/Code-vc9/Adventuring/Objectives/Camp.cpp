#include "stdafx.h"

#include "Objectives/Camp.h"
#include "Engine/Hero.h"

Camp::Camp(Hero& hero, float durationOfCamping) :
   m_hero(hero),
   m_durationOfCamping(0),
   m_targetDurationOfCamping(durationOfCamping)
{
   m_name = "Camping... ";
}
Camp::~Camp()
{}

static const float CampingEffectiveness = 1.0f;
void Camp::applyWork(float dt)
{
   m_hero.setState( Hero::Camping );
   m_durationOfCamping += dt;
   m_hero.applyRest(dt, CampingEffectiveness);
   m_hero.consumeProvisions(dt);
}
bool Camp::objectiveMet() const
{
   return m_durationOfCamping >= m_targetDurationOfCamping;
}


