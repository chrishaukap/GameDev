#include "stdafx.h"
#include "Cell.h"

void 
Cell::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
	const Vector2 screenPos = MathUtil::ScreenToWorld(screenCoords.X, screenCoords.Y);
	{
      if(m_playerOwned == TicTacToeGame::InvalidPlayer)
      {
		   const Vector2 size = GetSize();
		   const Vector2 position = GetPosition();
   	   
		   float lowerX = position.X - (0.5f * size.X);
		   float upperX = position.X + (0.5f * size.X);
		   float lowerY = position.Y - (0.5f * size.Y);
		   float upperY = position.Y + (0.5f * size.Y);

		   if(screenPos.X >= lowerX && screenPos.X <= upperX &&
			   screenPos.Y >= lowerY && screenPos.Y <= upperY )
		   {
            m_playerOwned = theTicTacToeGame.GetTurn();

				assert( TicTacToeGame::X == m_playerOwned );
			   if( m_playerOwned == TicTacToeGame::X )
				   SetColor( Color3f(1,0,0) );
			   //else
				//   SetColor( Color3f(0,1,0) );

            theTicTacToeGame.CheckForWins();
			   theTicTacToeGame.NextPlayersTurn();

				// HAUKAP -   the contents of this if block should go in game engine
				MiniMaxTree decisionTree;
				Cell** cells = theTicTacToeGame.getCells();
				unsigned int size = theTicTacToeGame.size();
				TTTGameState state(size);
				for(unsigned int i=0; i<size; ++i)
					for(unsigned int j=0; j<size; ++j)
						state.m_grid[j][i] = cells[j][i].getOwnership();

				TTTPlayerMove move = decisionTree.getBestMove(state, 1);
				assert( cells[move.m_j][move.m_i].getOwnership() == TicTacToeGame::InvalidPlayer );
            cells[move.m_j][move.m_i].setOwnership( TicTacToeGame::O );
            cells[move.m_j][move.m_i].SetColor( Color3f(0,1,0) );

            theTicTacToeGame.CheckForWins();
			   theTicTacToeGame.NextPlayersTurn();

		   }
      }
	}
}