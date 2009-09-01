#pragma once

#include "JakeGameManager.h"
#include "TextActor.h"

namespace CDH{
	namespace Chubdiso{
		class Game;
	}
}

class JakeGameScreen : 
   public GameScreen, 
   public CCmdMemberFunctorOwner
{
public:
	JakeGameScreen();

	virtual void Start();
	virtual void Update(float dt);
	virtual void Stop();
	void Render(); 

   // called every frame to cleanup stale entities
   void CleanUp();
   
private:
	CDH::Chubdiso::Game *m_game;
};
