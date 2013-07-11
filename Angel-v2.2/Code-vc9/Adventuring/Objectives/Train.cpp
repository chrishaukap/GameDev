#include "stdafx.h"

#include "Objectives/Train.h"
#include "Engine/Hero.h"
#include "Locations/Location.h"

std::string vocationToString(VocationType type)
{
   switch(type)
   {
   case Arcane: return std::string("Arcane");
   case Melee: return std::string("Melee");
   case Archery: return std::string("Archery");
   default: assert( ! "bad Vocation");
   }
}

Train::Train(Hero& hero, VocationType vocation, float durationOfTraining) :
   m_hero(hero),
   m_durationOfTraining(0),
   m_targetDurationOfTraining(durationOfTraining),
   m_vocation(vocation)
{
   assert(m_vocation != InvalidVocation);
   m_name = "Train in " + vocationToString(vocation);
}
Train::~Train()
{}

static const int ArcaneTrainingFatigueFactor = 50;
void Train::applyWork(float dt)
{
   switch(m_vocation)
   {
   case Arcane:
      if(m_hero.getCurrentLocation() != NULL &&
         m_hero.getCurrentLocation()->Type == Location::ArcaneSchool &&
         m_hero.getCurrentLocation()->Functional)
      {
         m_hero.setState( Hero::ArcaneTraining );

         m_durationOfTraining += dt;
         m_hero.applyArcaneExperience(dt, 5.0f /*HAUKAP - effectiveness of training based off of location*/);
         m_hero.applyFatigue( dt * ArcaneTrainingFatigueFactor );
      }
      else
      {
         //   we want to train in arcane but are not at a school!  what do we do??
         //         a. goto nearest school and try to train again ?
         //         b. abandon this objective and move onto the next objective ?

         m_cannotComplete = true; // mark it for removal
      }
      break;
   case Archery:      
      m_hero.setState( Hero::ArcheryTraining );
   case Melee:
      m_hero.setState( Hero::MartialTraining );
   default: assert(0);
   }
}
bool Train::objectiveMet() const
{
   return m_durationOfTraining >= m_targetDurationOfTraining;
}


