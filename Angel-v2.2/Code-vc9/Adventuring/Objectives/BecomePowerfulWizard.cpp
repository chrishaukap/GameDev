#include "stdafx.h"

#include "Objectives/BecomePowerfulWizard.h"
#include "Objectives/TravelToLocation.h"
#include "Objectives/Train.h"
#include "Engine/GameWorld.h"
#include "Locations/Location.h"
#include "Engine/Hero.h"

static Location* 
getNearestLocationByFilter(Location::LocationType type)
{
   //HAUKAP - returns first in list for now
   Locations& locations = theGameWorld.locations();
   Locations::iterator iter = locations.begin();
   while(iter != locations.end())
   {
      if( (*iter)->Type == type &&
          (*iter)->Functional )
         return (*iter);
      ++iter;
   }
   return NULL;
}
const static float traingingTIme = 6.0f;
const static float campingTime = 2.0f;
BecomePowerfulWizard::BecomePowerfulWizard(Hero& hero) :
   m_hero(hero)
{
   m_name = "Become A Powerful Wizard";

   Location* library = getNearestLocationByFilter(Location::Library);
   Location* arcaneSchool = getNearestLocationByFilter(Location::ArcaneSchool);
   assert( library  != NULL && arcaneSchool != NULL);
   m_prereqs.push_back(new TravelTo(m_hero, *arcaneSchool));
}
BecomePowerfulWizard::~BecomePowerfulWizard()
{}

static const int IntelligenceTarget = 100;
void BecomePowerfulWizard::applyWork(float dt)
{
   assert(m_prereqs.empty());
   if(m_hero.intelligence() < IntelligenceTarget ) // not powerful enough!
   {  
      if(m_hero.wantsToWork())
      {
         if(!m_hero.getCurrentLocation()->Functional)
         {
            Location* nearestSchool = 
               getNearestLocationByFilter(Location::ArcaneSchool);
            if(nearestSchool != NULL)
            {
               m_prereqs.push_back( 
                   new TravelTo( m_hero, *nearestSchool));
            }
            else
            {
               assert(!"no school to train at!");
            }
         }
         else
            m_prereqs.push_back(new Train(m_hero, Arcane, traingingTIme ));
      }
      else
      {
         assert(!"hero doesn't want to work!");
      }
   }
}

bool BecomePowerfulWizard::objectiveMet() const
{
   return m_prereqs.empty() && m_hero.intelligence() >= IntelligenceTarget ;
}