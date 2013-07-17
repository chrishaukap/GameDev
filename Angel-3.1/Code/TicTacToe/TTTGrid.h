#pragma once

#include "cdhBase.h"
#include "TTTPlayerMove.h"

namespace TicTacToe
{   
   class TTTGrid
   {
   public:
	   TTTGrid(unsigned int gridSize);
	   TTTGrid(const TTTGrid& copy);
	   ~TTTGrid();
      
      // caller responsible for deleting
      TTTGrid* clone() const;

      void setGridLocation(TTTPlayerMove move);
      inline unsigned int size() const {return m_gridSize;}
      inline const TTTPlayer& grid(unsigned int j, unsigned int i) const {return m_grid[j][i];}
      inline bool isCellEmpty(unsigned int j, unsigned int i) const 
      {
         bool isEmpty = m_grid[j][i] == Empty;
         return isEmpty;
      }

      void reset();
      bool CheckForWins(TTTPlayer player) const;
   private:
      void AllocGrid();
	   unsigned int m_gridSize;
	   TTTPlayer **m_grid;
     
      bool CheckHorizontal(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckVertical(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckDiagonals(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckForWinAroundCell(unsigned int j, unsigned int i, TTTPlayer player) const;
   };
}