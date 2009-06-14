#include "StdAfx.h"
#include "Game.h"
#include "Grid.h"
#include "Block.h"

#include <assert.h>
using namespace CDH;
using namespace Tetris;
#define DEBUG_BLOCKS

#define BLOCK_MOVE_SPEED 1.0f
#define BLOCK_FALL_SPEED 0.5f

const float Game::BlockMoveSpeed = BLOCK_MOVE_SPEED;
Game::Game(CHUint rows, CHUint cols) : 
	m_grid(new Grid(rows, cols)),
	m_activeBlock(NULL), 
	m_blocks(), 
	m_blockFallingSpeed(BLOCK_FALL_SPEED),
	m_state(Uninitialized)
{}
Game::~Game()
{
	delete m_grid; m_grid = NULL;
	delete m_activeBlock; m_activeBlock = NULL;
	
	int numBlocks = (int)m_blocks.size();
	for( int i=0; i<numBlocks; ++i )
		delete m_blocks[i];
	m_blocks.clear();
}

CHUint 
Game::numRows() const 
{
	return m_grid->rows();
}
CHUint 
Game::numCols() const
{
	return m_grid->cols();
}
const Block* 
Game::activeBlock() const 
{
	return m_activeBlock;
}
const std::vector<Block*>& 
Game::blockGrid() const 
{
	return m_blocks;
}
void 
Game::createNewActiveBlock()
{
	BlockType blockType = LineBlock;
	CHUint row = 0;
	CHUint col = CHUint((m_grid->cols()-1) * 0.5f);
#ifndef DEBUG_BLOCKS
	blockType = (BlockType)MathUtil::RandomIntInRange(0, NumBlockTypes);
#endif
	m_activeBlock = new Block(blockType, (float)col, (float)row);
}

void 
Game::addBlocksToBottomGrid()
{
	assert(m_activeBlock != NULL);
	m_blocks.push_back(m_activeBlock);

	const std::vector<Square>& squares = m_activeBlock->squares();
	for( std::vector<Square>::const_iterator iter = squares.begin();
		  iter != squares.end();
		  ++iter )
	{
		CHUint row, col;
		iter->SnappedCellPosition(col, row);
		Cell& cell = m_grid->cell(col,row);
		cell.setOccupyingBlock(m_activeBlock);
	}

	m_activeBlock = NULL;
}

void 
Game::moveActiveBlockDown()
{
   m_activeBlock->moveDown(BlockMoveSpeed, *m_grid);
}
void 
Game::moveActiveBlockLeft()
{
   m_activeBlock->moveLeft(BlockMoveSpeed, *m_grid);
}
void 
Game::moveActiveBlockRight()
{
   m_activeBlock->moveRight(BlockMoveSpeed, *m_grid);
}

bool
Game::rowsCompleted() const
{
	CHUint cols = m_grid->cols();
	CHUint rows = m_grid->rows();
	for( CHUint j=0; j<rows; ++j )
	{
		bool rowComplete = true;
		for( CHUint i=0; i<cols; ++i )
		{
			if(m_grid->cell(i,j).isEmpty())
			{
				rowComplete = false;
				break;
			}
		}
		if(rowComplete)
			m_rowsCompleted.push_back(j);
	}
	return !m_rowsCompleted.empty();
}

void 
Game::start()
{
	m_state = FallingBlock;
}
void 
Game::stop()
{
	m_state = Uninitialized;
}

void
Game::update(float dt)
{
	switch(m_state)
	{
	case FallingBlock: 
		{
			if(m_activeBlock == NULL)
				createNewActiveBlock();
			bool collision = m_activeBlock->moveDown(m_blockFallingSpeed * dt, *m_grid);
			if(collision)
			{
				addBlocksToBottomGrid();
				if(rowsCompleted())
					m_state = Cascading;
				else
					createNewActiveBlock();

			}
		}
		break;
	case Cascading:
			assert(!"not implemented");
		break;
	default: assert( !"unknown game state" );
	}
}