#include "StdAfx.h"

#include "vine.h"
#include "MathUtil.h"
#include "DrawUtil.h"

#include <vector>
#include <queue>
#include <list>

class VNode;
typedef std::vector<VNode*> VINENODES;
class VNode
{
public:
	VNode(const Vector2& p1, float angle) :
		m_p1(p1), m_angle(angle), m_length(0), m_children()
	{}
	~VNode()
	{
		int sz = (int)m_children.size();
		for( int i=0; i<sz; ++i )
			delete m_children[i];
		m_children.clear();
	}
	void grow(const Vector2& targetPoint, bool& reachedTarget,
				 VINENODES& newChildren, VINENODES& staleChildren)
	{
		reachedTarget = false;		
		incrementGrowth();
      Sleep(10);

		reachedTarget = bool(MathUtil::Distance(m_p1.X, m_p1.Y, targetPoint.X, targetPoint.Y) <= GrowthIncrement);
		if(maxLengthReached() && !reachedTarget)
		{
			staleChildren.push_back(this);
			addRandomizedChildren(endPoint(), targetPoint);
			int sz = (int)m_children.size();
			for( int i=0; i<sz; ++i )
				newChildren.push_back(m_children[i]);
		}		
	}
	void render() const
	{		
		DrawLine( m_p1, endPoint() );
		int numChildren = (int)m_children.size();
      for(int i=0; i<numChildren; ++i)
         m_children[i]->render();
	}
private:
	const Vector2 endPoint() const
	{
		Vector2 p2 = MathUtil::VectorFromAngle(m_angle);		
		return p2 * m_length;
	}
	void incrementGrowth()
	{
		m_length += GrowthIncrement;
	}
	bool maxLengthReached()
	{		
		return bool(m_length >= MaxLength);
	}
	void addRandomizedChildren(const Vector2& startPoint, const Vector2& targetPoint)
	{
		float angleInRadians = MathUtil::AngleFromVectors(startPoint, targetPoint);

		// n times...
		m_children.push_back( new VNode(startPoint, MathUtil::ToDegrees(angleInRadians) /*randomize this*/) );
	}

	static float MaxLength;
	static float GrowthIncrement;
	Vector2 m_p1;
	float m_angle;
	float m_length;
	std::vector<VNode*> m_children;
};
float VNode::MaxLength = 0.5f;
float VNode::GrowthIncrement = 0.1f;

class Vine::Data
{
public:
	Data() : 
		root(new VNode(Vector2(0.0f, 0.0f), 15)), 
		shoots(), points()
	{}
	~Data()
	{
		delete root; root = NULL;

		// HAUKAP - make sure no leak on queue
		while(!points.empty())
			points.pop();
		shoots.clear();
	}
	
	void cleanseChildren(VINENODES& newChildren, VINENODES& staleChildren)
	{
		removeStaleChildren(staleChildren);
		addNewChildren(newChildren);
	}
	void growShoots()
	{
		bool reachedTarget = false;
		VINENODES newChildren;
		VINENODES staleChildren;

		std::list<VNode*>::iterator iter = shoots.begin();
		while( iter != shoots.end() )
		{
			(*iter)->grow( *points.front(), reachedTarget, newChildren, staleChildren );			
			++iter;
		}

		if(reachedTarget)
			points.pop();
		cleanseChildren(newChildren, staleChildren);
	}
	VNode* root;
	std::list<VNode*> shoots;
	std::queue<Vector2*> points;
private:
	void removeStaleChildren(VINENODES& children)
	{
		// HAUKAP - may be slow if leaves gets huge
		int sz = (int)children.size();
		for( int i=0; i<sz; ++i )
			shoots.remove(children[i]);
	}
	void addNewChildren(VINENODES& children)
	{
		int sz = (int)children.size();
		for( int i=0; i<sz; ++i )
			shoots.push_back(children[i]);
	}
};

Vine::Vine() : 
	_vine(new Data())
{
	createPointList();
	_vine->shoots.push_back(_vine->root);
}
Vine::~Vine()
{
	delete _vine; _vine = NULL;
}
void Vine::update()
{
	_vine->growShoots();
}
void Vine::render() const
{
	_vine->root->render();
}
void Vine::createPointList()
{
	_vine->points.push(new Vector2(6.0f, 6.0f));
	_vine->points.push(new Vector2(-6.0f, 6.0f));
	_vine->points.push(new Vector2(-6.0f, -6.0f));
	_vine->points.push(new Vector2(8.0f, -6.0f));
	_vine->points.push(new Vector2(8.0f, 8.0f));
}