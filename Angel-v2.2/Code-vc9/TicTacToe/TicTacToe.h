#pragma once

#include "cdhBase.h"
#include "TTTGameState.h"

class Actor;
namespace TicTacToe
{	
	class TTTGridActor;
	class TTTGame
	{
	public:		
		friend class TTTGrid;
		static TTTGame &getInstance();
		TTTGame();
      ~TTTGame(){}
		void Destroy();

		void StartGame(float cellSize, unsigned int gridSize);
      void EndGame();

		void Process();

		TTTPlayer GetTurn() const;
		bool GameOver() const;
		
		void setPlayer1Type(bool isAI);
		void setPlayer2Type(bool isAI);
		
	   Color GetPlayerColor() const;
	   Color GetPlayerColor(TTTPlayer player) const;
      bool CellEmpty(CDH::CHUint j, CDH::CHUint i) const;

	private:
		static TTTGame *s_TTTGame;

		void NextPlayersTurn();

		// logic
		TTTGameState* m_gameState;
		//void SetPlayerTookTurn() {m_playerTookTurn = true;}
		bool m_playerTookTurn;

		// rendering
		TTTGridActor* m_gridActor;
	};

#define theTTTGame TTTGame::getInstance()
}