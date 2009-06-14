#pragma once

#include "TetrisGameManager.h"
#include "TextActor.h"

namespace CDH{
	class TetrisRenderer_OpenGL;
	namespace Tetris{
		class Game;
	}
}

class TetrisGameScreen : public GameScreen, public CCmdMemberFunctorOwner
{
public:
	TetrisGameScreen();

	virtual void Start();
	virtual void Update(float dt);
	virtual void Stop();
	void Render();
   
   void Rotate(const String& input);
   void MoveBlockLeft(const String& input);
   void MoveBlockRight(const String& input);
   void MoveBlockDown(const String& input);

private:
	TextActor *t;
	CDH::Tetris::Game *m_game;
	CDH::TetrisRenderer_OpenGL* m_tetrisRenderer;
};
