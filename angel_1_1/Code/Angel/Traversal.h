#pragma once

#include <hash_map>
#include <vector>
#include <deque>
#include "Vector2.h"

class SpatialGraphKDNode;

class Traversal
{
	typedef stdext::hash_map<SpatialGraphKDNode*, int> VisitedNeighborsTable;
	typedef std::deque<SpatialGraphKDNode*> CurrentNodesQueue;
public:

	Traversal()
		: m_bTraversalInitialized(false) {}

	virtual void StartTraversal( const Vector2& vStartPoint, int maxResults = -1, int maxIterations = -1 );
	virtual bool DoNextTraversal();
	virtual void ExecuteFullTraversal();

	virtual std::vector<Vector2>& GetResults();

protected:
	virtual void EvaluateNode( SpatialGraphKDNode* pNode ) = 0;
	virtual void AddSuccessors( SpatialGraphKDNode* pCurrent);

	virtual void AddNodeToVisit( SpatialGraphKDNode* pNode );
	virtual bool HasNodesToVist();
	virtual SpatialGraphKDNode* PopNextNode();
	virtual void SetVisited( SpatialGraphKDNode* pNode );
	virtual bool WasVisited( SpatialGraphKDNode* pNode );
	virtual void ClearAllVisited();
private:

	bool					m_bTraversalInitialized;
	int						m_maxIterations;
	int						m_numIterations;
	int						m_maxResults;
	std::vector<Vector2>	m_results;
	CurrentNodesQueue		m_nodesToVisit;
	VisitedNeighborsTable	m_visited;
};