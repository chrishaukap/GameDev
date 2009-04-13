#include "stdafx.h"
#include "Traversal.h"
#include "SpatialGraph.h"


void Traversal::StartTraversal( const Vector2& vStartPoint, int maxResults, int maxIterations )
{
	m_bTraversalInitialized = true;
	m_maxIterations = maxIterations;
	m_numIterations = 0;
	m_maxResults = maxResults;
	ClearAllVisited();
	m_results.clear();
	m_nodesToVisit.clear();

	//Add start node
	AddNodeToVisit(theSpatialGraph.GetGraph()->FindNode( vStartPoint ));
}

bool Traversal::DoNextTraversal()
{
	if( !m_bTraversalInitialized )
		return false;

	while( HasNodesToVist() )
	{
		//get next node
		SpatialGraphKDNode* pNextNode = PopNextNode();

		EvaluateNode(pNextNode);

		if( m_maxResults != -1 && m_results.size() >= m_maxResults )
			return false;

		m_numIterations++;
		if( m_maxIterations != -1 && m_numIterations >= m_maxIterations )
			return false;

	}

	return false;
}

void Traversal::ExecuteFullTraversal()
{
	while( DoNextTraversal() ) {}
}

std::vector<Vector2>& Traversal::GetResults()
{
	return m_results;
}

void Traversal::AddSuccessors( SpatialGraphKDNode* pCurrent)
{
	for( int i = 0; i < pCurrent->NeighborLOS.size(); i++ )
	{
		if( pCurrent->NeighborLOS[i] )
		{
			SpatialGraphKDNode* pNeighbor = pCurrent->Neighbors[i];
			if( WasVisited(pNeighbor) )
				AddNodeToVisit( pNeighbor );
		}
	}
}

void Traversal::AddNodeToVisit( SpatialGraphKDNode* pNode )
{
	m_nodesToVisit.push_back( pNode );
	SetVisited(pNode);
}

bool Traversal::HasNodesToVist()
{
	return m_nodesToVisit.size() > 0;
}

SpatialGraphKDNode* Traversal::PopNextNode()
{
	SpatialGraphKDNode* pNext = m_nodesToVisit.back();
	m_nodesToVisit.pop_front();

	return pNext;
}

void Traversal::SetVisited( SpatialGraphKDNode* pNode )
{
	m_visited[pNode] = 1;
}

bool Traversal::WasVisited( SpatialGraphKDNode* pNode )
{
	return m_visited.find(pNode) != m_visited.end();
}

void Traversal::ClearAllVisited()
{
	m_visited.clear();
}

