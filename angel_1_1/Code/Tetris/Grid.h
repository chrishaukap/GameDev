#ifndef GRID_H
#define GRID_H

#include "cdhBase.h"

#include <vector>

namespace CDH
{
	namespace Tetris
	{
		struct Block;
		struct Cell
		{
			Cell() : m_block(NULL) {}
			bool isEmpty() const { return m_block == NULL; }
			void setOccupyingBlock(Block* block) {m_block = block;}
		private:
			Block* m_block;
		};

		struct Grid
		{
			Grid::Grid(CHUint rows, CHUint cols) : 
				m_rows(rows), m_cols(cols)
			{
				m_grid = new Cell*[cols];
				for(CHUint i=0; i<m_cols; ++i)
					m_grid[i] = new Cell[m_rows];
			}
			Grid::~Grid()
			{
				for(CHUint i=0; i<m_rows; ++i)
				{
					delete m_grid[i]; m_grid[i] = NULL;
				}
				delete m_grid; m_grid = NULL;
			}

			CHUint rows() const {return m_rows;}
			CHUint cols() const {return m_cols;}
			const Cell& cell(CHUint col, CHUint row) const {return m_grid[col][row];}
			Cell& cell(CHUint col, CHUint row) {return m_grid[col][row];}

		private:
			CHUint m_rows, m_cols;
			Cell** m_grid;
		};
	}
}

#endif // GRID_H