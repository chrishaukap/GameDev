#pragma once

#include "cdhBase.h"

namespace CDH
{
	namespace Tetris{
		class Game;
	}
	class TetrisRenderer_OpenGL
	{
		public:
			TetrisRenderer_OpenGL(const Tetris::Game& game);
			~TetrisRenderer_OpenGL();

			void render() const;

		private:
			const Tetris::Game& m_game;
			TetrisRenderer_OpenGL(const TetrisRenderer_OpenGL& C);
			const TetrisRenderer_OpenGL& operator=(const TetrisRenderer_OpenGL& C);
	};
}