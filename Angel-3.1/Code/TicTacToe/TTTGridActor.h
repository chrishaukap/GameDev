#pragma once

#include "Actors/GridActor.h"
#include "Input/MouseInput.h"
#include "TTTPlayerMove.h"

class Actor;
namespace TicTacToe
{
	class TTTGridActor : public GridActor, public MouseListener
	{
	public:
		TTTGridActor(float cellSize, unsigned int gridSize);
		~TTTGridActor();
		void DrawAxes(){}

      void AddCellsToWorld();
      void RemoveCellsFromWorld();
		void RefreshCells();

		void MouseDownEvent(Vec2i screenCoords, MouseButtonInput button);
		const TTTPlayerMove& getMostRecentMove() const {return m_mostRecentMove;}
		void acceptUserInput(bool accept);
		bool hasPlayerTakenTurn() const;
		void makeMove( const TTTPlayerMove& move );
	private:		
		void SetDimensions(float width, float height);
		Actor** m_cells;
		float m_cellSize;
		unsigned int m_gridSize;
		
		TTTPlayerMove m_mostRecentMove;
		bool m_collectUserInput;
	};		
}