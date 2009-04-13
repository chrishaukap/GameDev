#pragma once

#include "cdhBase.h"
#include "TTTPlayerMove.h"

namespace TicTacToe
{   
   class TTTGrid
   {
   public:
	   TTTGrid(CDH::CHUint gridSize);
	   TTTGrid(const TTTGrid& copy);
	   ~TTTGrid();
      
      // caller responsible for deleting
      TTTGrid* clone() const;

      void setGridLocation(TTTPlayerMove move);
      inline CDH::CHUint size() const {return m_gridSize;}
      inline const TTTPlayer& grid(CDH::CHUint j, CDH::CHUint i) const {return m_grid[j][i];}
      inline bool isCellEmpty(CDH::CHUint j, CDH::CHUint i) const 
      {
         bool isEmpty = m_grid[j][i] == Empty;
         return isEmpty;
      }

      void reset();
      bool CheckForWins(TTTPlayer player) const;
   private:
      void AllocGrid();
	   CDH::CHUint m_gridSize;
	   TTTPlayer **m_grid;
     
      bool CheckHorizontal(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckVertical(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckDiagonals(unsigned int j, unsigned int i, TTTPlayer player) const;
      bool CheckForWinAroundCell(unsigned int j, unsigned int i, TTTPlayer player) const;
   };
}