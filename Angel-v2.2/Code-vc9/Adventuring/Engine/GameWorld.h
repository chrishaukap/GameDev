#pragma once

#include <list>
#include <map>

class Hero;
class Location;
class Monster;

#define theGameWorld GameWorld::GetInstance()
typedef std::list<class Location*> Locations;
typedef std::list<class Monster*> Monsters;
typedef std::map<int,int> ArcaneLevelThresholdMap;


class GameWorld
{
public:
   ~GameWorld();
	static GameWorld &GetInstance();

   void DebugInit();
   const Hero& hero() const;
   

   void step(float dt);
   const Locations& locations() const;
   Locations& locations();
   const Monsters& monsters() const;
   Monsters& monsters();

   
   const ArcaneLevelThresholdMap& arcaneLevelThresholds() const;
private:
	GameWorld();
   void UpdateCombats(float dt);
   void CleanupWorld();
	static GameWorld *s_GameWorld;
	   
   Locations m_locations;
   Monsters m_monsters;
   Hero *m_hero;
};
