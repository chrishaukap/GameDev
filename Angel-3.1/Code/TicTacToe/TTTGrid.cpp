#include "stdafx.h"
#include "TTTGrid.h"

using namespace TicTacToe;

TTTGrid::TTTGrid(unsigned int gridSize) : 
	m_gridSize(gridSize), 
	m_grid(NULL)
{
	AllocGrid();
}
   
TTTGrid::TTTGrid(const TTTGrid& copy) :
   m_gridSize(copy.m_gridSize)
{
   AllocGrid();
   for(unsigned int i=0; i<m_gridSize; ++i)
      for(unsigned int j=0; j<m_gridSize; ++j)
	      m_grid[j][i] = copy.m_grid[j][i];		
}

TTTGrid* TTTGrid::clone() const
{
   return new TTTGrid(*this);
}
      
TTTGrid::~TTTGrid()
{		
	for(unsigned int i=0; i<m_gridSize; ++i )
		delete [] m_grid[i];
	delete [] m_grid;
}		

void
TTTGrid::setGridLocation(TTTPlayerMove move)
{
   m_grid[move.m_j][move.m_i] = move.m_turn;
}
void
TTTGrid::reset()
{   
	for(unsigned int i=0; i<m_gridSize; ++i)
		for(unsigned int j=0; j<m_gridSize; ++j)
			m_grid[j][i] = Empty;		
}
void
TTTGrid::AllocGrid()
{
	m_grid = new TTTPlayer* [m_gridSize];
	for(unsigned int i=0; i<m_gridSize; ++i)
		m_grid[i] = new TTTPlayer[m_gridSize];
}


bool 
TTTGrid::CheckHorizontal(unsigned int j, unsigned int i, TTTPlayer player) const
{
   if( int(i)-2 >= 0 &&  i+2 < m_gridSize )
   {
      if( m_grid[j][i] == player &&
          m_grid[j][i+1] == player &&
          m_grid[j][i-1] == player &&
          m_grid[j][i+2] == player &&
          m_grid[j][i-2] == player )
          return true;
   }
   return false;
}
bool
TTTGrid::CheckVertical(unsigned int j, unsigned int i, TTTPlayer player) const
{
   if( int(j)-2 >= 0 && j+2 < m_gridSize )
   {
      if( m_grid[j][i] == player &&
          m_grid[j+1][i] == player &&
          m_grid[j-1][i] == player &&
          m_grid[j+2][i] == player &&
          m_grid[j-2][i] == player )
          return true;
   }
   return false;
}

bool 
TTTGrid::CheckDiagonals(unsigned int j, unsigned int i, TTTPlayer player ) const
{
   
   if( int(i)-2 >= 0 && int(j)-2 >= 0 &&
       i+2 < m_gridSize && j+2 < m_gridSize )
   {
      if( m_grid[j][i] == player &&
          m_grid[j+1][i-1] == player &&
          m_grid[j+2][i-2] == player &&
          m_grid[j-1][i+1] == player &&
          m_grid[j-2][i+2] == player )
          return true;
      if( m_grid[j][i] == player &&
          m_grid[j+1][i+1] == player &&
          m_grid[j-1][i-1] == player &&
          m_grid[j+2][i+2] == player &&
          m_grid[j-2][i-2] == player )
          return true;
   }
   return false;

}
bool
TTTGrid::CheckForWinAroundCell(unsigned int j, unsigned int i, TTTPlayer player) const
{
   if(m_grid[j][i] == player)
   {
      if(CheckHorizontal(j,i,player))
         return true;
      if(CheckVertical(j,i,player))
         return true;
      if(CheckDiagonals(j,i,player))
         return true;
   }
   return false;
}
bool 
TTTGrid::CheckForWins(TTTPlayer player) const
{
   bool win = false;
	for(unsigned int i=0; i< m_gridSize; ++i)
		for(unsigned int j=0; j< m_gridSize; ++j)
         if(CheckForWinAroundCell(j,i, player))
				return true;
	return false;
}