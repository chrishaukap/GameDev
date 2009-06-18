#ifndef TETRISGAME_H
#define TETRISGAME_H

#include "cdhBase.h"

#include <vector>

namespace CDH
{
	namespace Tetris
	{
		struct Grid;
		struct Block;
		class Game
		{
		public:
			Game(CHUint rows, CHUint cols);
			~Game();

         void update(float dt);
			void start();
			void stop();
         void moveActiveBlockDown();
         void moveActiveBlockLeft();
         void moveActiveBlockRight();

			// for rendering
			CHUint numRows() const;
			CHUint numCols() const;
			const Block* activeBlock() const;
			const std::vector<Block*>& blockGrid() const;
						
		private:
			void createNewActiveBlock();
			void addBlocksToBottomGrid();
			bool rowsCompleted() const;
         void doCascade();
         void removeCompletedRows();
         void cascadeBlocks();

			Block* m_activeBlock;
			std::vector<Block*> m_blocks;
			mutable std::vector<CHUint> m_rowsCompleted;
         float m_blockFallingSpeed;
			Grid* m_grid;

			enum GameState
			{
				Uninitialized = -1,
				FallingBlock,
				Cascading,
			};
			GameState m_state;

         static const float BlockMoveSpeed;
		};
	}
}

#endif // TETRISGAME_H