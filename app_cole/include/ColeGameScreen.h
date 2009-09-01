#pragma once

#include "ColeGameManager.h"
#include "TextActor.h"

namespace CDH{
	namespace Cole{
		class Game;
	}
}

class ColeGameScreen : 
   public GameScreen, 
   public CCmdMemberFunctorOwner
{
public:
	ColeGameScreen();

	virtual void Start();
	virtual void Update(float dt);
	virtual void Stop();
	void Render(); 

   // called every frame to cleanup stale entities
   void CleanUp();
   
private:
	void UpdateLinearVelocity();
	CDH::Cole::Game *m_game;
};
