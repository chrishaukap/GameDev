#include "stdafx.h"

#include "TicTacToe.h"
#include "MathUtil.h"
#include "World.h"
#include "TicTacToeGameManager.h"
#include "MouseInput.h"
#include "GridActor.h"
#include "Actor.h"
#include "minimax.h"
#include "TTTGridActor.h"

#define DECISION_DEPTH 2
using namespace CDH;
using namespace TicTacToe;

TTTGame* TTTGame::s_TTTGame = NULL;
TTTGame& TTTGame::getInstance()
{
	if (s_TTTGame == NULL)
	{
		s_TTTGame = new TTTGame();
	}
	return *s_TTTGame;
}
void
TTTGame::Destroy()
{
	delete m_gridActor;
	delete m_gameState;
   delete s_TTTGame;
}
TTTGame::TTTGame() : 
	m_gridActor(NULL),
	m_gameState(NULL),
	m_playerTookTurn(false)
{}
void TTTGame::StartGame(float cellSize, unsigned int gridSize)
{
	if(m_gameState == NULL)
		m_gameState = new TTTGameState(gridSize,X);
	else
		m_gameState->ResetState(X);
   
	if(m_gridActor == NULL)
		m_gridActor = new TTTGridActor(cellSize, gridSize);
	else
		m_gridActor->RefreshCells();

	theWorld.Add(m_gridActor,1);
   m_gridActor->AddCellsToWorld();
}
void
TTTGame::EndGame()
{
   m_gridActor->RemoveCellsFromWorld();
   theWorld.Remove(m_gridActor);
}

void TTTGame::Process()
{
	// 1) current player
	// 2)
	// if player is human, set grid to receive mouse input
	// else if player is AI, get AI's best move
	bool playerTookTurn = false;
	if( m_gameState->currentPlayerIsHuman() )
	{
		if(m_gridActor->hasPlayerTakenTurn())
		{
			const TTTPlayerMove move = m_gridActor->getMostRecentMove();
			m_gameState->makeMove(move);;
			playerTookTurn = true;
		}

		// this call will throw away whatever turn has been taken, check for the turn before calling it.
		m_gridActor->acceptUserInput(true);
	}
	else
	{
		// player is AI
      TTTPlayerMove move = MiniMaxTree::getBestMove( *m_gameState, DECISION_DEPTH);
		if(move.m_turn == Invalid)
      {
         m_gameState->setTurn( Invalid );
         m_gameState->setGameOver(); // draw
         return; 
      }

      m_gameState->makeMove(move);
	   m_gridActor->makeMove(move);
		playerTookTurn = true;
	}

	if(playerTookTurn)
	{
		// 3) check for wins for current player
		bool gameOver = m_gameState->CheckForWins();
      if(gameOver)
         m_gameState->setGameOver();

		// 4) switch players
		if(!gameOver)
			NextPlayersTurn();		
	}
}

void 
TTTGame::NextPlayersTurn()
{
	m_gameState->setTurn( (m_gameState->turn() == X) ? O : X );
}

TTTPlayer 
TTTGame::GetTurn() const 
{
	return m_gameState->turn();
}
bool
TTTGame::GameOver() const
{
	return m_gameState->gameOver();
}

bool 
TTTGame::CellEmpty(CHUint j, CHUint i) const
{
	return m_gameState->isCellEmpty(j,i);
}

void TTTGame::setPlayer1Type(bool isAI)
{
	m_gameState->setPlayer1Type(isAI);
}
void TTTGame::setPlayer2Type(bool isAI)
{
	m_gameState->setPlayer2Type(isAI);
}


Color3f TTTGame::GetPlayerColor() const
{
	return GetPlayerColor(m_gameState->turn());
}
Color3f TTTGame::GetPlayerColor(TTTPlayer player) const
{
	return m_gameState->GetPlayerColor(player);
}