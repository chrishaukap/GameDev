#pragma once

#include "GridActor.h"
#include "MouseInput.h"
#include "TTTPlayerMove.h"

class Actor;
namespace TicTacToe
{
	class TTTGridActor : public GridActor, public MouseListener
	{
	public:
		TTTGridActor(float cellSize, unsigned int gridSize);
		~TTTGridActor();
		Vector2 getLLCorner() const;
		Vector2 getURCorner() const;
		void DrawAxes(){}

      void AddCellsToWorld();
      void RemoveCellsFromWorld();
		void RefreshCells();

		void MouseDownEvent(Vec2i screenCoords, MouseButtonInput button);
		const TTTPlayerMove& getMostRecentMove() const {return m_mostRecentMove;}
		void acceptUserInput(bool accept);
		bool hasPlayerTakenTurn() const;
		void makeMove( const TTTPlayerMove& move );
		//Actor& Cell(unsigned int j, unsigned int i) ;
	private:		
		void SetDimensions(float width, float height);
		Actor** m_cells;
		float m_cellSize;
		unsigned int m_gridSize;
		
		TTTPlayerMove m_mostRecentMove;
		bool m_collectUserInput;
	};		
}