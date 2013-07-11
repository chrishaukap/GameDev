#include "stdafx.h"

#include "Locations/GreatLibrary.h"

GreatLibrary::GreatLibrary(std::string name) : Location(name)
{
   Type = Location::Library;
}
GreatLibrary::~GreatLibrary()
{}

// Actor overrides
void GreatLibrary::Render()
{
   Location::Render();
}

void GreatLibrary::Update(float dt)
{

}
