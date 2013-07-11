#include "stdafx.h"

#include "Locations/Location.h"

static int idCounter = 0;
Location::Location(std::string name) : 
   m_name(),
   Type(Invalid)
{
   SetName(name);
   char buf[99999];
   sprintf(buf, "%d", idCounter++);
   m_name = name;
   m_name += buf;
}
Location::~Location()
{}

// Actor overrides
void Location::Render()
{
   Actor::Render();
}

void Location::Update(float dt)
{

}
