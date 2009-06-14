#include "StdAfx.h"
#include "Block.h"
#include "Grid.h"

using namespace CDH;
using namespace Tetris;

Block::Block(BlockType type, float col, float row) :
   m_type(type)
{      
   makeSelf(col, row);
}
void
Block::makeSelf(float col, float row)
{   
   switch( m_type )
   {
   case SquareBlock:
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col+1, (float)row+1));
      m_squares.push_back(Square( (float)col+1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      break;
   case LineBlock: 
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      m_squares.push_back(Square( (float)col, (float)row+2));
      m_squares.push_back(Square( (float)col, (float)row+3));		
		break;
      
   case LeftCorner:
      m_squares.push_back(Square( (float)col-1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      m_squares.push_back(Square( (float)col, (float)row+2));	
      break;
   case RightCorner:
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col+1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      m_squares.push_back(Square( (float)col, (float)row+2));	
      break;
   case LeftSlant:
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col-1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      m_squares.push_back(Square( (float)col+1, (float)row+1));	
      break;
   case RightSlant:
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col+1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));
      m_squares.push_back(Square( (float)col-1, (float)row+1));	
      break;
   case TriangleBlock:
      m_squares.push_back(Square( (float)col, (float)row));
      m_squares.push_back(Square( (float)col-1, (float)row));
      m_squares.push_back(Square( (float)col+1, (float)row));
      m_squares.push_back(Square( (float)col, (float)row+1));	
      break;

   default: break;
   }
}
void
Block::moveSquares( float diffColumns, float diffRows )
{
   for( std::vector<Square>::iterator iter = m_squares.begin();
        iter != m_squares.end();
        ++iter )
   {
      iter->m_row += diffRows;
      iter->m_col += diffColumns;
   }
}
bool 
Block::moveLeft(float amount, const Grid& grid)
{
   moveSquares( -amount, 0.0f );
	if(isCollision(grid))
	{
		snapSquaresRight();
		return true;
	}
	else
		return false;
}
bool 
Block::moveRight(float amount, const Grid& grid)
{
   moveSquares( amount, 0.0f );
	if(isCollision(grid))
	{
		snapSquaresLeft();
		return true;
	}
	else
		return false;
} 
bool 
Block::moveDown(float amount, const Grid& grid)
{
   moveSquares( 0.0f, amount );
	if(isCollision(grid))
	{
		snapSquaresUp();
		return true;
	}
	else
		return false;
}
void
Block::moveUp(float amount, const Grid& grid)
{
   moveSquares( 0.0f, -amount );
}
void 
Block::snapSquaresUp()
{
   for( std::vector<Square>::iterator iter = m_squares.begin();
        iter != m_squares.end();
        ++iter )
   {
		float floorRow = floor(iter->m_row);
		if(MathUtil::FuzzyEquals( iter->m_row, floorRow ))
			iter->m_row -= 1.0f;
		else
			iter->m_row = floor(iter->m_row);
   }
}
void 
Block::snapSquaresLeft()
{	
   for( std::vector<Square>::iterator iter = m_squares.begin();
        iter != m_squares.end();
        ++iter )
   {
		iter->m_col = floor(iter->m_col-1);
   }
}
void 
Block::snapSquaresRight()
{	
   for( std::vector<Square>::iterator iter = m_squares.begin();
        iter != m_squares.end();
        ++iter )
   {
		iter->m_col = ceil(iter->m_col+1);
   }
}

bool
Block::isCollision(const Grid& grid) const
{
   for( std::vector<Square>::const_iterator iter = m_squares.begin();
        iter != m_squares.end();
        ++iter )
   {
      CHUint col,row;
      iter->SnappedCellPosition(col,row);

      if( col < 0 || col >= grid.cols() )
          return true;

      if( row < 0 || row >= grid.rows() )
          return true;

      if( ! grid.cell(col, row).isEmpty() )
         return true;
   }
   return false;
}
