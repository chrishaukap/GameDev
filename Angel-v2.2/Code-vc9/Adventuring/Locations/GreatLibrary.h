#pragma once

#include "Location.h"

class GreatLibrary : public Location
{
public:
   GreatLibrary(std::string name);
   virtual ~GreatLibrary();

   // Actor overrides
   virtual void Render();
   virtual void Update(float dt);
   
private:
};
