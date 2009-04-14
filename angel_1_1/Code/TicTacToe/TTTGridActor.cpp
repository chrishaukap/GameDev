#include "stdafx.h"
#include "TTTGridActor.h"

#include "Actor.h"
#include "World.h"
#include "TicTacToe.h"
#include "TTTPlayerMove.h"

using namespace TicTacToe;

TTTGridActor::TTTGridActor(float cellSize, unsigned int gridSize) : 
	m_cells(NULL), m_cellSize(cellSize), m_gridSize(gridSize), m_collectUserInput(false)
{						
	SetDimensions(gridSize * cellSize, gridSize * cellSize);
	SetInterval(cellSize);
	SetLineColor( Color3f(0,0,0) );

	m_cells = new Actor* [gridSize];
	for(unsigned int i=0; i< gridSize; ++i)
		m_cells[i] = new Actor[gridSize];
}

TTTGridActor::~TTTGridActor()
{		
	for(unsigned int i=0; i< m_gridSize; ++i )
		delete [] m_cells[i];
	delete [] m_cells;
}

void
TTTGridActor::AddCellsToWorld()
{
	Vector2 llCorner = getLLCorner();
	float x = llCorner.X + (m_cellSize * 0.5f);
	float y = llCorner.Y + (m_cellSize * 0.5f);
	for(unsigned int i=0; i<m_gridSize; ++i)
		for(unsigned int j=0; j<m_gridSize; ++j)
		{
			m_cells[j][i].SetPosition( x + m_cellSize*i, y + m_cellSize*j);
			m_cells[j][i].SetColor(Color3f(0.75f,0.75f,0.75f));
			m_cells[j][i].SetSize(m_cellSize);
			theWorld.Add( &(m_cells[j][i]) );
		}
}
void
TTTGridActor::RemoveCellsFromWorld()
{
   for(unsigned int i=0; i<m_gridSize; ++i)
      for(unsigned int j=0; j<m_gridSize; ++j)
	      theWorld.Remove( &(m_cells[j][i]) );
}
void
TTTGridActor::RefreshCells()
{
   for(unsigned int i=0; i<m_gridSize; ++i)
      for(unsigned int j=0; j<m_gridSize; ++j)
			m_cells[j][i].SetColor(Color3f(0.75f,0.75f,0.75f));
}

void TTTGridActor::SetDimensions(float width, float height)
{		
	_minCoord = Vector2( -(width * 0.5f), -(height * 0.5f));
	_maxCoord = Vector2( width * 0.5f, height * 0.5f );
}
Vector2 TTTGridActor::getLLCorner() const {return _minCoord;}
Vector2 TTTGridActor::getURCorner() const {return _maxCoord;}

void TTTGridActor::acceptUserInput(bool accept)
{
	m_collectUserInput = accept;
	m_mostRecentMove.m_i = 0;
	m_mostRecentMove.m_j = 0;
	m_mostRecentMove.m_turn = Invalid;
}
void TTTGridActor::makeMove( const TTTPlayerMove& move )
{
	m_cells[move.m_j][move.m_i].SetColor( theTTTGame.GetPlayerColor( move.m_turn ) );
}
bool TTTGridActor::hasPlayerTakenTurn() const
{
	return (m_mostRecentMove.m_turn != Invalid);
}
void TTTGridActor::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)			
{
	if(m_collectUserInput)
	{
		const Vector2 screenPos = MathUtil::ScreenToWorld(screenCoords.X, screenCoords.Y);
		{
			Vector2 llCorner = getLLCorner();
			Vector2 urCorner = getURCorner();
			if(screenPos.X >= llCorner.X && screenPos.X <= urCorner.X &&
				screenPos.Y >= llCorner.Y && screenPos.Y <= urCorner.Y)
			{
				// in the grid... now find the specific cell
				unsigned int j = unsigned int((screenPos.Y - llCorner.Y) / m_cellSize);
				unsigned int i = unsigned int((screenPos.X - llCorner.X) / m_cellSize);
				if( theTTTGame.CellEmpty(j,i) )
				{
					m_mostRecentMove.m_i = i;
					m_mostRecentMove.m_j = j;
					m_mostRecentMove.m_turn = theTTTGame.GetTurn();
					makeMove( m_mostRecentMove );
					m_collectUserInput = false;
				}
			}
		}
	}
}
/*
Actor& TTTGridActor::Cell(unsigned int j, unsigned int i) 
{
	return m_cells[j][i];
}*/