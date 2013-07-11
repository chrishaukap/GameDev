#include "stdafx.h"

#include "Objectives/TravelToLocation.h"
#include "Objectives/Camp.h"
#include "Locations/Location.h"
#include "Engine/Hero.h"

TravelTo::TravelTo(Hero& hero, Location &location) : 
   m_hero(hero), 
   m_location(location)
{
   m_name = "Travel To: " + m_location.GetName();
}
static const float fatigueMultiplier = 100.0f;
void TravelTo::applyWork(float dt)
{
   m_hero.setState( Hero::Travel );

   Vector2 heropos = m_hero.GetPosition();
   Vector2 locpos = m_location.GetPosition();
   Vector2 movevec;

   movevec.X = locpos.X - heropos.X;
   movevec.Y = locpos.Y - heropos.Y;
   movevec.Normalize();

   const float speed = 3.0f;
   heropos.X += movevec.X * dt * speed;
   heropos.Y += movevec.Y * dt * speed;
   m_hero.SetPosition(heropos);
   m_hero.applyFatigue( fatigueMultiplier  * dt);
}
bool TravelTo::objectiveMet() const 
{
   const float errorThreshold = 0.055f;
   if(m_hero.GetPosition().isEqual(m_location.GetPosition(), errorThreshold))
   {
      m_hero.setCurrentLocation(m_location);
      return true;
   }
   else return false;
}
