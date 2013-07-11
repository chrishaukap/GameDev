#pragma once

#include "Location.h"

class ArcaneSchool : public Location
{
public:
   ArcaneSchool(std::string name);
   virtual ~ArcaneSchool();

   // Actor overrides
   virtual void Render();
   virtual void Update(float dt);
   
private:
   
   void CheckDestruction(float dt);
   float m_timeSpentResearching;
   int m_destroyCounter;
};
