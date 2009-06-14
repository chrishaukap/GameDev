#include "stdafx.h"
#include "TetrisRenderer_OpenGL.h"
#include "Game.h"
#include "Block.h"
#include "DrawUtil.h"
#include <assert.h>

using namespace CDH;
using namespace Tetris;

#define OFFSET_X -5
#define OFFSET_Y 9

template<typename type> 
void GridToPosition( type row, type col, Vector2& ul, Vector2& lr )
{
	float size = 1;
	ul.X = OFFSET_X + col * size;
	ul.Y = OFFSET_Y - row * size;
	lr.X = OFFSET_X + (col+1) * size;
	lr.Y = OFFSET_Y - (row+1) * size;	
}

static Color3f BlockTypeToColor(BlockType type)
{
	switch(type)
	{
	case SquareBlock: return Color3f(1.0f, 0,0);
	case LineBlock: return Color3f(0,1.0f,0);
	case LeftCorner: return Color3f(0,0,1.0f);
	case RightCorner: return Color3f(1.0f,0,1.0f);
	case LeftSlant: return Color3f(1.0f, 1.0f,0);
	case RightSlant: return Color3f(0,1.0f, 1.0f);
	case TriangleBlock: return Color3f(1.0f, 0.5f, 0.5f);
	default: assert(0); break;
	}
	return Color3f(0.0f, 0.0f, 0.0f);
}

TetrisRenderer_OpenGL::TetrisRenderer_OpenGL(const Game& game) :
	m_game(game)
{}

TetrisRenderer_OpenGL::~TetrisRenderer_OpenGL(){}

void 
TetrisRenderer_OpenGL::render() const
{
	Vector2 ul, lr;
	CHUint cols = m_game.numCols();
	CHUint rows = m_game.numRows();
	for (CHUint i=0; i<rows; ++i)
	{
		for (CHUint j=0; j<cols; ++j)
		{
			GridToPosition(i, j, ul, lr);
			DrawSquare( ul, lr, Color3f(0.8f,0.8f,0.8f));
		}
	}
	
	// draw active block
	const Block* activeBlock = m_game.activeBlock();
	if(activeBlock)
	{
		const std::vector<Square>& squares = activeBlock->squares();
		for( std::vector<Square>::const_iterator iter = squares.begin();
			  iter != squares.end();
			  ++iter )
		{
			float row = iter->m_row, col = iter->m_col;
			GridToPosition(row,col, ul, lr);
			DrawSquare( ul, lr, BlockTypeToColor(activeBlock->type()));
		}
	}

	// draw bottom grid of blocks
	const std::vector<Block*>& blockGrid = m_game.blockGrid();
	int numBlocks = (int)blockGrid.size();
	for(int i=0; i<numBlocks; ++i)
	{
		const std::vector<Square>& squares = blockGrid[i]->squares();
		for( std::vector<Square>::const_iterator iter = squares.begin();
			  iter != squares.end();
			  ++iter )
		{
			float row = iter->m_row, col = iter->m_col;
			GridToPosition(row,col, ul, lr);
			DrawSquare( ul, lr, BlockTypeToColor(blockGrid[i]->type()));
		}
	}	
}