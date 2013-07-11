#include "stdafx.h"

#include "Locations/ArcaneSchool.h"
#include "Engine/GameWorld.h"

#include "Engine/Hero.h"
ArcaneSchool::ArcaneSchool(std::string name) : 
   Location(name),
   m_destroyCounter(0),
   m_timeSpentResearching(0)
{
   Type = Location::ArcaneSchool;
}
ArcaneSchool::~ArcaneSchool()
{}

// Actor overrides
void ArcaneSchool::Render()
{
   Location::Render();
}

void ArcaneSchool::CheckDestruction(float dt)
{
   const Location* location = theGameWorld.hero().getCurrentLocation();
   if(this == location) // if hero is at *this* location
   {
      m_timeSpentResearching += dt;
      int accumSeconds = (int)m_timeSpentResearching ;
      if( accumSeconds - m_destroyCounter >= 1)
      {
         float roll = MathUtil::RandomFloat(100.0f);
         if(roll > 50.0f)
            Functional = false;
         ++m_destroyCounter ;
      }
   }

}
void ArcaneSchool::Update(float dt)
{   
   CheckDestruction(dt);
}
