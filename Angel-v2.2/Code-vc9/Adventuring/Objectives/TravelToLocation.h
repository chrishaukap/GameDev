#pragma once

#include "Objectives/Objective.h"

class Hero;
class Location;

class TravelTo : public Objective
{
public:
   TravelTo(Hero& hero, Location &location);
   void applyWork(float dt);
protected:
   bool objectiveMet() const ;
private:
   Hero& m_hero;
   Location& m_location;   
};