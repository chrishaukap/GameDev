#pragma once

#include "GridActor.h"



#define theTicTacToeGame TicTacToeGame::getInstance()

class TicTacToeGrid;
class Cell;

class TicTacToeGame
{
public:
	static TicTacToeGame &getInstance();
	enum Player{ X = 0, O, InvalidPlayer };
	TicTacToeGame();
	~TicTacToeGame();

	void InitGame(float cellSize, unsigned int gridSize);
   void EndGame();

	void NextPlayersTurn() {m_turn = (m_turn == X) ? O : X;}
	Player GetTurn() const {return m_turn;}
   void CheckForWins();
private:
	static TicTacToeGame *s_TicTacToeGame;

	Player m_turn;
	TicTacToeGrid* m_gridActor;
	Cell** m_cells;
	unsigned int m_gridSize;
	float m_cellSize;
   
   bool CheckHorizontal(unsigned int j, unsigned int i);
   bool CheckVertical(unsigned int j, unsigned int i);
   bool CheckDiagonals(unsigned int j, unsigned int i);
   bool CheckForWinAroundCell(unsigned int j, unsigned int i);
};