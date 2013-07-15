#pragma once

#include "EdgeGameManager.h"
#include "TextActor.h"

namespace CDH{
	namespace Edge{
		class Game;
	}
}

class EdgeGameScreen : 
   public GameScreen, 
   public CCmdMemberFunctorOwner
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
	CDH::Edge::Game *m_game;
};
