#ifndef BLOCK_H
#define BLOCK_H

#include "cdhBase.h"

#include <vector>

namespace CDH
{
	namespace Tetris
	{
		enum BlockType
		{
			SquareBlock,
			LineBlock,
         LeftCorner,
         RightCorner,
         LeftSlant,
         RightSlant,
         TriangleBlock,
			NumBlockTypes
		};

		struct Square
		{
			Square(float col, float row) : m_col(col), m_row(row) {}

			// snaps to the ceiling of row
			void SnappedCellPosition(CHUint& col, CHUint& row) const 
			{ 
				col = (CHUint)m_col; 
				row = (CHUint)ceil(m_row); 
			}
			float m_row, m_col;
		};

		struct Grid;
		struct Block
		{
			Block(BlockType type, float col, float row);

			// returns whether the move resulted in a collision (the collision has already been responded too)
         void moveUp(float amount, const Grid& grid);
         bool moveDown(float amount, const Grid& grid);
         bool moveLeft(float amount, const Grid& grid); 
         bool moveRight(float amount, const Grid& grid);

         const BlockType& type() const {return m_type;}
         const std::vector<Square>& squares() const {return m_squares;}
      private:
			BlockType m_type;
			std::vector<Square> m_squares;

         void moveSquares( float diffColumns, float diffRows );
         void makeSelf(float col, float row);
			bool isCollision(const Grid& grid) const;
			void snapSquaresUp();
			void snapSquaresLeft();
			void snapSquaresRight();
		};
	}
}

#endif // BLOCK_H