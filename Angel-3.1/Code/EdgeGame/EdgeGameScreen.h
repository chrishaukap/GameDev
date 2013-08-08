#pragma once

#include "EdgeGameManager.h"
#include "Actors/TextActor.h"

namespace Edge{
	class Game;
}

class EdgeGameScreen : 
   public GameScreen
{
public:
	EdgeGameScreen();

	virtual void Start();
	virtual void Update(float dt);
	virtual void Stop();
	void Render(); 

   // called every frame to cleanup stale entities
   void CleanUp();
   
private:
	Edge::Game *m_game;
};
