#pragma once

#include "EdgeGameManager.h"
#include "TextActor.h"

namespace CDH{
	namespace Edge{
		class Game;
	}
}

class EdgeGameScreen : public GameScreen, public CCmdMemberFunctorOwner
{
public:
	EdgeGameScreen();

	virtual void Start();
	virtual void Update(float dt);
	virtual void Stop();
	void Render();
   
   void Rotate(const String& input);
   void MoveBlockLeft(const String& input);
   void MoveBlockRight(const String& input);
   void MoveBlockDown(const String& input);

private:
	CDH::Edge::Game *m_game;
};
