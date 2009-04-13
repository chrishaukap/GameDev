#include "stdafx.h"
#include "SpatialGraph.h"
#include "MathUtil.h"
#include "TextRendering.h"
#include "StringUtil.h"
#include "Console.h"
#include "World.h"
#include "Ray2.h"
#include "DrawUtil.h"
#include "Box2D.h"

void SpatialGraphKDNode::Render()
{
	if( HasChildren() )
	{
		LHC->Render();
		RHC->Render();
		return;
	}

	CONSOLE_DECLAREVAR( sg_drawBounds );
	CONSOLE_DECLAREVAR( sg_drawBlocked );
	CONSOLE_DECLAREVAR( sg_drawGridPoints );
	CONSOLE_DECLAREVAR( sg_drawGraph );
	CONSOLE_DECLAREVAR( sg_drawNodeIndex );

	if( sg_drawBlocked->GetIntVal() )
	{
		if( bBlocked )
		{
			glColor4f(1,0,0,0.25f);
			BBox.RenderBox();
		}
	}

	if( sg_drawBounds->GetIntVal() )
	{
		glColor4f(0,0,0,1.f);
		BBox.RenderOutline();
	}


	Vector2 centroid = BBox.Centroid();

	if( sg_drawNodeIndex->GetIntVal() )
	{
		Vector2 screenCenter = MathUtil::WorldToScreen( centroid.X, centroid.Y );
		//Print some vals
		glColor3f(0,1.f,1.f);
		DrawGameText( IntToString(Index), "ConsoleSmall", (int)screenCenter.X, (int)screenCenter.Y );
	}

	if( sg_drawGraph->GetIntVal() && !bBlocked )
	{
		glColor3f(1.f,0.f,0.f);

		for( int i = 0; i < Neighbors.size(); i++ )
		{
			if( Neighbors[i]->bBlocked || !NeighborLOS[i] )
				continue;
			//Draw lines
			glBegin(GL_LINES);
			//draw centroid to centroid half way point
			Vector2 neighbor = Neighbors[i]->BBox.Centroid();
			neighbor = centroid + ((neighbor - centroid) * 0.6f);

			glVertex2f(centroid.X, centroid.Y);
			glVertex2f(neighbor.X, neighbor.Y);

			glEnd();
		}
	}

	if( sg_drawGridPoints->GetIntVal() )
	{
		glColor3f(1.f,0.f,0.f);
		Vector2List gridPoints;
		int xPoints, yPoints;
		GetGridPoints(gridPoints, xPoints, yPoints );

		for( int i = 0; i < gridPoints.size(); i++ )
		{
			DrawPoint( gridPoints[i], Tree->GetSmallestDimensions().X * 0.15f );
		}

	}
}

void SpatialGraphKDNode::GetGridPoints( Vector2List& points, int& xPoints, int& yPoints )
{
	xPoints = 0;
	yPoints = 0;

	Vector2 vSmallestDimensions = Tree->GetSmallestDimensions();
	Vector2 vMyBoxDimensions = BBox.Max - BBox.Min;

	/*
	if( vSmallestDimensions == vMyBoxDimensions )
	{
		xPoints = 1;
		yPoints = 1;
		points.push_back( BBox.Centroid() );
		return;
	}
	*/

	xPoints = static_cast<int>(vMyBoxDimensions.X / vSmallestDimensions.X);
	yPoints = static_cast<int>(vMyBoxDimensions.Y / vSmallestDimensions.Y);
	points.reserve(xPoints*yPoints);

	Vector2 vBottomLeftStartBox( BBox.Min.X, BBox.Max.Y - vSmallestDimensions.Y );

	BoundingBox startBox( vBottomLeftStartBox, vBottomLeftStartBox + vSmallestDimensions);

	BoundingBox checkBox( startBox );

	for( int yDim = 0; yDim < yPoints; yDim++ )
	{
		for( int xDim = 0; xDim < xPoints; xDim++ )
		{
			points.push_back( checkBox.Centroid() );

			checkBox.Min.X += vSmallestDimensions.X;
			checkBox.Max.X += vSmallestDimensions.X;
		}

		checkBox.Min.X = startBox.Min.X;
		checkBox.Max.X = startBox.Max.X;

		checkBox.Min.Y -= vSmallestDimensions.Y;
		checkBox.Max.Y -= vSmallestDimensions.Y;
	}

}


SpatialGraph::SpatialGraph(float entityWidth, const BoundingBox& startBox )
{
	_entityWidth = entityWidth;
	float maxDimension = MathUtil::Max( startBox.Max.Y - startBox.Min.Y, startBox.Max.X - startBox.Min.X );
	int depth = 0;
	while( maxDimension > _entityWidth )
	{
		maxDimension /= 2.0f;
		depth += 2;
	}
	_depth = MathUtil::Max(depth,1);
	if( _depth > 24 )
		_depth = 24;

	int depthMask = ~(0xFFFFFFFF << _depth);

	_dirMasks[0] = 0x1;
	_dirMasks[1] = 0x2;
	_dirMasks[2] = 0xaaaaaaaa & depthMask;
	_dirMasks[3] = _dirMasks[2] >> 1;

	_root = CreateTree(_depth+1, startBox, NULL);

	//Get smallest dimension
	_smallestDimensions = startBox.Max - startBox.Min;
	for( int i = 0; i < _depth; i++ )
	{
		if( i % 2 )
			_smallestDimensions.Y *= 0.5f;
		else
			_smallestDimensions.X *= 0.5f;
	}

	ComputeNeighbors( _root );
	ValidateNeighbors( _root );
}

SpatialGraph::~SpatialGraph()
{
	DeleteNode( _root );
}

void SpatialGraph::DeleteNode( SpatialGraphKDNode* pNode )
{
	if( pNode == NULL )
		return;

	DeleteNode( pNode->LHC );
	DeleteNode( pNode->RHC );

	delete pNode;
}



SpatialGraphKDNode* SpatialGraph::FindNode(SpatialGraphKDNode* node, const BoundingBox& bbox)
{
	if (node == NULL)
		return NULL;
	//check if this bbox fits entirely within our node
	if (node->BBox.Contains(bbox) == Within)
	{
		//Check our children
		SpatialGraphKDNode* retVal = FindNode(node->LHC, bbox );
		if (retVal != NULL)
			return retVal;
		retVal = FindNode(node->RHC, bbox);
		if( retVal != NULL )
			return retVal;

		//otherwise, return ourselves
		return node;

	}
	return NULL;
}

SpatialGraphKDNode* SpatialGraph::FindNode(SpatialGraphKDNode* node, const Vector2& point)
{
	if (node == NULL)
		return NULL;
	//check if this bbox fits entirely within our node
	if (node->BBox.Contains(point))
	{
		//Check our children
		SpatialGraphKDNode* retVal = FindNode(node->LHC, point );
		if (retVal != NULL)
			return retVal;
		retVal = FindNode(node->RHC, point);
		if( retVal != NULL )
			return retVal;

		//otherwise, return ourselves
		return node;

	}
	return NULL;
}


SpatialGraphKDNode* SpatialGraph::FindNode(const BoundingBox& bbox)
{
	return FindNode(_root, bbox);
}

SpatialGraphKDNode* SpatialGraph::FindNode(const Vector2& point)
{
	return FindNode(_root, point);
}

void SpatialGraph::Render()
{
	CONSOLE_DECLAREVAR( sg_drawBounds );
	CONSOLE_DECLAREVAR( sg_drawBlocked );
	CONSOLE_DECLAREVAR( sg_drawGridPoints );
	CONSOLE_DECLAREVAR( sg_drawGraph );

	bool bDrawAny = sg_drawBounds->GetIntVal() || sg_drawGridPoints->GetIntVal() || sg_drawGraph->GetIntVal() || sg_drawBlocked->GetIntVal();

	if( bDrawAny && _root)
	{
		_root->Render();
	}
}

bool IsBlocked( const BoundingBox& bbox )
{
	const int maxShapeCount = 1024;
	b2AABB physBounds;
	physBounds.lowerBound = b2Vec2( bbox.Min.X, bbox.Min.Y ); 
	physBounds.upperBound = b2Vec2( bbox.Max.X, bbox.Max.Y );
	static b2Shape* tempShapes[maxShapeCount];

	int numBroadphase = theWorld.GetPhysicsWorld().Query( physBounds, tempShapes, maxShapeCount );

	//No bodies here
	if( numBroadphase == 0 )
		return false;

	b2PolygonDef shapeBoundsDef;
	shapeBoundsDef.vertexCount = 4;
	shapeBoundsDef.vertices[0].Set( physBounds.lowerBound.x, physBounds.lowerBound.y );
	shapeBoundsDef.vertices[1].Set( physBounds.upperBound.x, physBounds.lowerBound.y );
	shapeBoundsDef.vertices[2].Set( physBounds.upperBound.x, physBounds.upperBound.y );
	shapeBoundsDef.vertices[3].Set( physBounds.lowerBound.x, physBounds.upperBound.y );

	b2BodyDef fakeBodyDef;
	//b2Vec2 center = physBounds.lowerBound + (0.5f * shapeBoundsDef.extents);
	fakeBodyDef.position.Set(0.0f, 0.0f );
	b2Body fakeBody(&fakeBodyDef, b2Body::e_staticType, &theWorld.GetPhysicsWorld() );
	b2PolygonShape shapeBounds( &shapeBoundsDef );

	for( int i = 0; i < numBroadphase; i++ )
	{
		b2Shape* pSh = tempShapes[i];
		if( pSh->GetType() == e_polygonShape  )
		{
			b2PolygonShape* pPolyShape = (b2PolygonShape*)pSh;

			b2Manifold m0;
			b2CollidePolygons(&m0, &shapeBounds, fakeBody.m_xf, pPolyShape, pPolyShape->GetBody()->m_xf );

			if( m0.pointCount > 0 )
				return true;

		}
		else if( pSh->GetType() == e_circleShape )
		{
			b2CircleShape* pCircleShape = (b2CircleShape*)pSh;
			b2Manifold m0;
			b2CollidePolygonAndCircle( &m0, &shapeBounds, fakeBody.m_xf, pCircleShape, pCircleShape->GetBody()->m_xf );
			if( m0.pointCount > 0 )
				return true;
		}
	}

	return false;

}

SpatialGraphKDNode* SpatialGraph::CreateTree(int depth, const BoundingBox& bbox, SpatialGraphKDNode* parent, int index)
{
	SpatialGraphKDNode* node = new SpatialGraphKDNode(bbox, parent);
	node->Tree = this;
	node->bBlocked = false;

	//query physics to see if we're blocked
	node->bBlocked = IsBlocked( bbox );

	//Calculate my index
	if( parent )
	{
		node->Index = index;
	}
	else
	{
		node->Index = 0;
	}

	//Bail out if we reach max depth
	depth--;
	node->Depth = _depth - depth;
	if (depth > 0 && node->bBlocked )
	{
		BoundingBox LHSbbox, RHSbbox;
		MathUtil::SplitBoundingBox( bbox, depth % 2 ? MathUtil::AA_X : MathUtil::AA_Y, LHSbbox, RHSbbox );
		node->LHC = CreateTree(depth, LHSbbox, node, node->Index << 1);
		node->RHC = CreateTree(depth, RHSbbox, node, (node->Index << 1) + 1);

		int iMask = ~(0xFFFFFFFF << depth );
		//If I have children, pad my index
		node->Index = (node->Index << depth) | iMask;

		//If all my children are blocked, then destroy my children
		if( IsFullyBlocked(node) )
		{
			DeleteNode( node->LHC );
			node->LHC = NULL;
			DeleteNode( node->RHC );
			node->RHC = NULL;
		}
	}

	return node;
}

bool SpatialGraph::IsFullyBlocked( SpatialGraphKDNode* node )
{
	if( node == NULL )
		return true;

	return node->bBlocked && IsFullyBlocked(node->LHC) && IsFullyBlocked(node->RHC);
}

#include <hash_map>

bool QuickContainsNode( stdext::hash_map<SpatialGraphKDNode*, int>& NodeList, SpatialGraphKDNode* pNode )
{
	stdext::hash_map<SpatialGraphKDNode*, int>::iterator itr = NodeList.find( pNode );
	return itr != NodeList.end();
}

void NudgePointOnPlane( const BoundingBox& BBox, Vector2& vPointOnPlane )
{
	//Get off the x planes
	if( vPointOnPlane.X == BBox.Min.X )
	{
		vPointOnPlane.X += MathUtil::Epsilon;
	}
	else if( vPointOnPlane.X == BBox.Max.X )
	{
		vPointOnPlane.X -= MathUtil::Epsilon;
	}

	//Get off the Y planes
	if( vPointOnPlane.Y == BBox.Min.Y )
	{
		vPointOnPlane.Y += MathUtil::Epsilon;
	}
	else if( vPointOnPlane.Y == BBox.Max.Y )
	{
		vPointOnPlane.Y -= MathUtil::Epsilon;
	}
}


bool SpatialGraph::CanGo( const Vector2& vFrom, const Vector2 vTo )
{

	//Get source cell
	SpatialGraphKDNode* pSourceNode = FindNode( vFrom );
	if( pSourceNode == NULL || pSourceNode->bBlocked)
		return false;

	SpatialGraphKDNode* pDestNode = FindNode( vTo );
	if( pDestNode == NULL || pDestNode->bBlocked)
		return false;

	return CanGoInternal( vFrom, vTo, pSourceNode, pDestNode );
}

bool SpatialGraph::CanGoInternal( const Vector2& vFrom, const Vector2 vTo, SpatialGraphKDNode* pSourceNode, SpatialGraphKDNode* pDestNode )
{
	//If source is dest, we definitely can go (we're both within bounding box)
	if( pSourceNode == pDestNode )
		return true;

	Vector2 vUseFrom = vFrom;
	Vector2 vUseTo = vTo;

	NudgePointOnPlane( pSourceNode->BBox, vUseFrom );
	NudgePointOnPlane( pDestNode->BBox, vUseTo );

	Ray2 ray = Ray2::CreateRayFromTo( vUseFrom, vUseTo );

	stdext::hash_map<SpatialGraphKDNode*, int> NodeList;
	SpatialGraphKDNode* pCurrent = pSourceNode;

	while( true )
	{
		//Mark current as visited
		NodeList[pCurrent] = 1;
		SpatialGraphKDNode* pNearestNeighbor = NULL;
		float fNearestNeighborDistance = MathUtil::MaxFloat;
		//iterate over currents neighbors to see if they intersect the ray
		for( int i = 0; i < pCurrent->Neighbors.size(); i++ )
		{
			SpatialGraphKDNode* pNeighbor = pCurrent->Neighbors[i];

			//Ignore neighbors we've already visited
			if( QuickContainsNode( NodeList, pNeighbor) )
				continue;

			float fDistanceAlongRay;
			if( pNeighbor->BBox.Intersects( ray, fDistanceAlongRay ) )
			{
				if( fDistanceAlongRay < fNearestNeighborDistance )
				{
					fNearestNeighborDistance = fDistanceAlongRay;
					pNearestNeighbor = pNeighbor;
				}
			}
		}

		//If we couldn't find a nearest neighbor, or the neighbor is blocked bail out
		if( pNearestNeighbor == NULL || pNearestNeighbor->bBlocked )
			break;

		//If the nearest neighbor is our destination, we found it!
		if( pNearestNeighbor == pDestNode )
			return true;

		//otherwise, check our neighbor
		pCurrent = pNearestNeighbor;
	}

	return false;
}

bool SpatialGraph::CanGoNodeToNode( SpatialGraphKDNode* pSourceNode, SpatialGraphKDNode* pDestNode )
{
	return CanGoInternal( pSourceNode->BBox.Centroid(), pDestNode->BBox.Centroid(), pSourceNode, pDestNode );
}


void SpatialGraph::AddNeighbor( SpatialGraphKDNode* node, const Vector2& pos )
{
	SpatialGraphKDNode* pNeighbor = node->Tree->FindNode( pos );
	if( pNeighbor )
	{
		//Add unique
		for( int i = 0; i < node->Neighbors.size(); i++ )
		{
			if( node->Neighbors[i] == pNeighbor )
				return;
		}

		node->Neighbors.push_back( pNeighbor );
		node->NeighborLOS.push_back(true);
	}

}

int GetColumnMajorIndex( int wantX, int wantY, int maxX )
{
	//Get column
	return (wantY * maxX) + wantX;
}

void SpatialGraph::ComputeNeighbors( SpatialGraphKDNode* node )
{
	if( node->HasChildren() )
	{
		ComputeNeighbors(node->LHC);
		ComputeNeighbors(node->RHC);
		return;
	}

	Vector2 checkN = Vector2::UnitY * _smallestDimensions.Y;
	Vector2 checkS = Vector2::UnitY * -_smallestDimensions.Y;
	Vector2 checkE = Vector2::UnitX * _smallestDimensions.X;
	Vector2 checkW = Vector2::UnitX * -_smallestDimensions.X;

	Vector2 centroid = node->BBox.Centroid();

	Vector2List gridPoints;
	int xPoints, yPoints;
	node->GetGridPoints(gridPoints, xPoints, yPoints );

	//Check north neighbors
	for( int i = 0; i < xPoints; i++ )
	{
		AddNeighbor( node, gridPoints[GetColumnMajorIndex(i,0,xPoints)] + checkN );
	}

	//Check south neighbors
	for( int i = 0; i < xPoints; i++ )
	{
		AddNeighbor( node, gridPoints[GetColumnMajorIndex(i,yPoints-1,xPoints)] + checkS );
	}

	//Check east neighbors
	for( int i = 0; i < yPoints; i++ )
	{
		AddNeighbor( node, gridPoints[GetColumnMajorIndex(xPoints-1,i,xPoints)] + checkE );
	}

	//Check west neighbors
	for( int i = 0; i < yPoints; i++ )
	{
		AddNeighbor( node, gridPoints[GetColumnMajorIndex(0,i,xPoints)] + checkW );
	}

}

void SpatialGraph::ValidateNeighbors( SpatialGraphKDNode* node )
{
	if( node->HasChildren() )
	{
		ValidateNeighbors(node->LHC);
		ValidateNeighbors(node->RHC);
		return;
	}
	//Validate neighbors
	for (int i = 0; i < node->Neighbors.size(); i++ )
	{
		//Todo, incorporate entity width
		if( !CanGoNodeToNode( node, node->Neighbors[i]) )
		{
			node->NeighborLOS[i] = false;
		}
	}
}


SpatialGraphManager* SpatialGraphManager::s_SpatialGraphManager = NULL;
SpatialGraphManager & SpatialGraphManager::GetInstance()
{
	if( s_SpatialGraphManager == NULL)
	{
		s_SpatialGraphManager = new SpatialGraphManager();
		s_SpatialGraphManager->Initialize();
	}
	return *s_SpatialGraphManager;
}

SpatialGraphManager::SpatialGraphManager()
: _spatialGraph(NULL)
{

}

SpatialGraphManager::~SpatialGraphManager()
{

}

void BuildSpatialGraph( const String& input )
{
	float entityWidth = 0.3f;
	BoundingBox bounds(Vector2(-20), Vector2(20));

	StringList args = SplitString(input);
	if( args.size() > 0 )
	{
		float customWidth = StringToFloat(args[0]);
		if( customWidth > 0.0f )
			entityWidth = customWidth;
	}

	theSpatialGraph.CreateGraph( entityWidth, bounds);
}


void SpatialGraphManager::CreateGraph( float entityWidth, const BoundingBox& bounds )
{
	if( _spatialGraph != NULL )
		delete _spatialGraph;

	_spatialGraph = new SpatialGraph( entityWidth, bounds );
}

//Vector2List s_tempPath;
void SpatialGraphManager::Render()
{

	if( _spatialGraph  )
		_spatialGraph->Render();

	CONSOLE_DECLAREVAR(sg_astar_start);
	CONSOLE_DECLAREVAR(sg_astar_end);

	bool bHasStartEnd = sg_astar_start->HasVal() && sg_astar_end->HasVal();

	if( bHasStartEnd )
	{
		glColor3f(0,0,1.f);
		DrawPoint( sg_astar_start->GetVector2Val(), 0.2f );
		DrawPoint( sg_astar_end->GetVector2Val(), 0.2f );
	}

}

bool ContainsNode( SpatialGraphNeighborList& path, const SpatialGraphKDNode* node)
{
	for( SpatialGraphNeighborList::iterator itr = path.begin(); itr != path.end(); itr++ )
	{
		if( (*itr) == node )
			return true;
	}

	return false;
}

#include "stlastar.h"

struct SearchInterface
{
	SpatialGraphKDNode* pNode;

	SearchInterface( SpatialGraphKDNode* node )
	{
		pNode = node;
	}

	SearchInterface()
	{
		pNode = NULL;
	}

	~SearchInterface()
	{
		int i = 2 + 2;
	}

	float GoalDistanceEstimate( const SearchInterface& goal )
	{
		return GetCost( goal );
	}

	bool IsGoal( const SearchInterface& goal )
	{
		return IsSameState(goal);
	}

	bool IsSameState( const SearchInterface& goal )
	{
		return pNode == goal.pNode;
	}

	bool GetSuccessors( AStarSearch<SearchInterface>* pSearch, SearchInterface* pParent )
	{
		for( int i = 0; i < pNode->Neighbors.size(); i++ )
		{
			SpatialGraphKDNode* successor = pNode->Neighbors[i];
			if( (!pParent || pParent->pNode != successor) && !successor->bBlocked && pNode->NeighborLOS[i])
			{
				if( !pSearch->AddSuccessor( SearchInterface(successor)) )
					return false;
			}
		}
		return true;
	}

	float GetCost( const SearchInterface& successor )
	{
		return Vector2::Distance( pNode->BBox.Centroid(), successor.pNode->BBox.Centroid() );
	}
};

bool ComputeAStar( SpatialGraphKDNode* pSourceNode, SpatialGraphKDNode* pDestNode, Vector2List& path )
{
	AStarSearch<SearchInterface> search;
	SearchInterface pStart(pSourceNode);
	SearchInterface pEnd(pDestNode);
	
	search.SetStartAndGoalStates( pStart, pEnd );

	while( AStarSearch<SearchInterface>::SEARCH_STATE_SEARCHING == search.SearchStep() )
	{

	}

	int curState = search.GetState();
	if( curState == AStarSearch<SearchInterface>::SEARCH_STATE_SUCCEEDED )
	{
		//Get path
		for( SearchInterface* pCur = search.GetSolutionStart(); pCur != NULL; pCur = search.GetSolutionNext() )
		{
			path.push_back( pCur->pNode->BBox.Centroid() );
		}
		search.FreeSolutionNodes();
		return true;
	}

	return false;
}

bool SpatialGraphManager::GetPath( const Vector2& source, const Vector2& dest, Vector2List& path )
{
	if( _spatialGraph == NULL )
		return false;

	//Get source cell
	SpatialGraphKDNode* pSourceNode = _spatialGraph->FindNode( source );
	if( pSourceNode == NULL )
		return false;

	SpatialGraphKDNode* pDestNode = _spatialGraph->FindNode( dest );
	if( pDestNode == NULL )
		return false;

	path.push_back(source);
	if( pSourceNode == pDestNode )
	{
		path.push_back(dest);
		return true;
	}

	//Compute A*
	bool retVal = ComputeAStar(pSourceNode, pDestNode, path);
	if( retVal == false )
	{
		path.clear();
		return false;
	}

	//otherwise, put dest on the pathlist
	path.push_back(dest);

	return true;
}

#if 0 //rb - Did this get purposely removed in Tim's branch?  tf - yes, but I'll keep the code around for testing
void SpatialGraphManager::CalcTempAStar()
{
	CONSOLE_DECLAREVAR(sg_astar_start);
	CONSOLE_DECLAREVAR(sg_astar_end);

	bool bHasStartEnd = sg_astar_start->HasVal() && sg_astar_end->HasVal();
	bool bHasChanged = false;

	CONSOLE_ONCVARCHANGED(sg_astar_start, Vector2,
	{
		bHasChanged = true;
	});

	CONSOLE_ONCVARCHANGED(sg_astar_end, Vector2,
	{
		bHasChanged = true;
	});

	if( bHasStartEnd && bHasChanged )
	{
		s_tempPath.clear();

		GetPath( sg_astar_start->GetVector2Val(), sg_astar_end->GetVector2Val(), s_tempPath );
	}
}
#endif 

bool SpatialGraphManager::CanGo( const Vector2& from, const Vector2 to )
{

	if( _spatialGraph == NULL )
		return false;

	return _spatialGraph->CanGo( from, to );

}

bool SpatialGraphManager::IsInPathableSpace( const Vector2& point )
{
	return CanGo(point,point);
}

bool SpatialGraphManager::FindNearestNonBlocked( const Vector2& fromPoint, Vector2& goTo )
{
	if( _spatialGraph == NULL )
		return false;

	SpatialGraphKDNode* pCurrentNode = _spatialGraph->FindNode( fromPoint );
	if( pCurrentNode == NULL )
		return false;

	float fMinDistance = MathUtil::MaxFloat;
	SpatialGraphKDNode* pNearestNeighbor = NULL;
	//otherwise, iterate over neighbors to find a non-blocked
	for( int i = 0; i < pCurrentNode->Neighbors.size(); i++ )
	{
		SpatialGraphKDNode* pNeighbor = pCurrentNode->Neighbors[i];
		if( pNeighbor->bBlocked )
			continue;

		Vector2 vDir = pNeighbor->BBox.Centroid() - fromPoint;
		Ray2 ray( fromPoint, Vector2::Normalize(vDir) );

		float distanceToBBox;
		if( pNeighbor->BBox.Intersects(ray, distanceToBBox) )
		{
			if( distanceToBBox < fMinDistance )
			{
				fMinDistance = distanceToBBox;
				pNearestNeighbor = pNeighbor;
			}
		}
	}

	if( pNearestNeighbor != NULL )
	{
		goTo = pNearestNeighbor->BBox.Centroid();
		return true;
	}

	return false;
}


void SpatialGraphManager::Initialize()
{
	CONSOLE_DECLARECMDSTATIC( BuildSpatialGraph, BuildSpatialGraph );

}

