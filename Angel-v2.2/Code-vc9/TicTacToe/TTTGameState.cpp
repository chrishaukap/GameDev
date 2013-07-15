#include "stdafx.h"
#include "TTTGameState.h"
#include "TTTGrid.h"
#include <assert.h>

using namespace CDH;
using namespace TicTacToe;

TTTPlayer TTTGameState::getOtherPlayer(TTTPlayer currentPlayer) 
{
	if(currentPlayer == X)
		return O;
	else
		return X;
	assert(0);
}

TTTGameState::TTTGameState(unsigned int gridSize, TTTPlayer player) : 	 
	m_grid(NULL), 
	m_turn(player),
	m_gameOver(false)
{
   m_grid = new TTTGrid(gridSize);
	ResetState(m_turn);	
}
TTTGameState::~TTTGameState()
{		
   delete m_grid; m_grid = NULL;
}		

void TTTGameState::ResetState(TTTPlayer turn)
{	
   m_grid->reset();
	m_turn = turn;
	m_gameOver = false;
}

Color TTTGameState::GetPlayerColor() const
{
	return GetPlayerColor(m_turn);
}
Color TTTGameState::GetPlayerColor(TTTPlayer player) const
{
	switch(player)
	{
	case X: return Color(1,0,0); 
	case O: return Color(0,1,0);
	default: assert( !"Invalid Player" );
	}
	return Color(0,0,0);
}

void
TTTGameState::makeMove(TTTPlayerMove move)
{
   assert(m_grid != NULL);
   m_grid->setGridLocation(move);
}
bool
TTTGameState::CheckForWins() const
{
   return m_grid->CheckForWins(turn());
}
bool
TTTGameState::isCellEmpty(CDH::CHUint j, CDH::CHUint i) const 
{
   return m_grid->isCellEmpty(j,i);
}
bool 
TTTGameState::currentPlayerIsHuman() const
{
	// HAUKAP - assuming X is player 1 & O is player 2
	switch(m_turn)
	{
	case X: return !m_player1Type;
	case O: return !m_player2Type;
	default: assert( !"Invalid Player" );		
	}
	return false;
}