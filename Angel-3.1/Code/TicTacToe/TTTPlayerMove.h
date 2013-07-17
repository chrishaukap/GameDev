#pragma once

#include "cdhBase.h"
namespace TicTacToe
{

enum TTTPlayer{ X = 0, O, Invalid, Empty = Invalid };

struct TTTPlayerMove
{
   const static TTTPlayerMove EMPTY;

	TTTPlayerMove() : m_j(0), m_i(0), m_turn(Invalid) {}
   TTTPlayerMove(unsigned int  j, unsigned int  i, TTTPlayer turn) : 
		m_j(j), m_i(i), m_turn(turn)	
	{}

	unsigned int m_j,m_i;
	TTTPlayer m_turn;
};

}