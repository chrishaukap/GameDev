#pragma once

#include "cdhBase.h"
#include "TTTPlayerMove.h"

namespace TicTacToe
{ 
   class TTTGrid;
   class TTTGameState
   {
   public:
	   TTTGameState(unsigned int gridSize, TTTPlayer player);
	   ~TTTGameState();
	   void ResetState(TTTPlayer);

	   static TTTPlayer getOtherPlayer(TTTPlayer currentPlayer) ;

	   void setTurn(TTTPlayer turn) {m_turn = turn;}
	   TTTPlayer turn() const {return m_turn;}

	   Color GetPlayerColor() const;
	   Color GetPlayerColor(TTTPlayer player) const;

	   bool gameOver() const {return m_gameOver;}
	   void setGameOver() {m_gameOver = true;}

	   void setPlayer1Type(bool isAI){m_player1Type = isAI;}
	   void setPlayer2Type(bool isAI){m_player2Type = isAI;}
	   bool currentPlayerIsHuman() const;

      void makeMove(TTTPlayerMove move);
      bool isCellEmpty(unsigned int j, unsigned int i) const;
      
		bool CheckForWins() const;
      inline TTTGrid& grid() {return *m_grid;}

   private:
      TTTGrid* m_grid;
	   TTTGameState();
	   TTTPlayer m_turn;
	   bool m_gameOver;
	   bool m_player1Type;
	   bool m_player2Type;
   };

}