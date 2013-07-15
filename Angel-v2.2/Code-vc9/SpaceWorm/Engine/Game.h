#pragma once

#include <vector>

#define theGame SpaceWormGame::GetInstance()
typedef std::vector<class Planet*> Planets;

class SpaceWormGame
{
public:
   ~SpaceWormGame();
	static SpaceWormGame &GetInstance();    

   const Planets& planets() const;
   Planets& planets();

   const class Worm& worm() const;
   class Worm& worm();

   const class ParticleActor& starEngine() const;
   class ParticleActor& starEngine();

   const class FullScreenActor& background() const;
   class FullScreenActor& background();
   
private:
	SpaceWormGame();
	static SpaceWormGame *s_SpaceWormGame;
	   
	ParticleActor* m_starEngine;
   FullScreenActor* m_background;
   Planets m_planets;
   class Worm *m_worm;
};
