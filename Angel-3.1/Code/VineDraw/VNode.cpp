#include "StdAfx.h"

#include "VNode.h"
#include "Util/DrawUtil.h"
#include "Infrastructure/Color.h"

VNode::VNode(const Vector2& location, const Vector2& orientation) :
	m_location(location), m_endPoint(0,0), m_orientation(orientation), m_length(0), m_children()
{
	m_orientation.Normalize();
}
VNode::~VNode()
{
	int sz = (int)m_children.size();
	for( int i=0; i<sz; ++i )
		delete m_children[i];
	m_children.clear();
}
void 
VNode::grow(const Vector2& targetPoint, bool& reachedTarget,
			   VINENODES& newChildren, VINENODES& staleChildren)
{
	incrementGrowth();
   setEndPoint();

   float distance = sqrt( pow((targetPoint.X - endPoint().X),2) + pow((targetPoint.Y - endPoint().Y),2) );
	reachedTarget = bool(distance <= GrowthIncrement);	

	if(!reachedTarget)
	{
		if(maxLengthReached())
		{
			staleChildren.push_back(this);
			addRandomizedChildren(endPoint(), targetPoint);
			int sz = (int)m_children.size();
			for( int i=0; i<sz; ++i )
				newChildren.push_back(m_children[i]);
		}		
	}
}
void 
VNode::render() const
{		
   glColor3f(0,1,0);
   glLineWidth(MaxWidth);
	DrawLine( m_location, endPoint() );
	int numChildren = (int)m_children.size();
   for(int i=0; i<numChildren; ++i)
      m_children[i]->render();
}

void 
VNode::addRandomizedChildren(const Vector2& startPoint, const Vector2& targetPoint)
{
   static const float fudgeFactor = 0.3f;
	Vector2 orientation = targetPoint - startPoint;
   orientation.Normalize();
   orientation.X = MathUtil::RandomFloatInRange( orientation.X - fudgeFactor, orientation.X + fudgeFactor );
   orientation.Y = MathUtil::RandomFloatInRange( orientation.Y - fudgeFactor, orientation.Y + fudgeFactor );

	// n times...
	m_children.push_back( new VNode(startPoint, orientation) );
}

float VNode::MaxLength = 0.5f;
float VNode::GrowthIncrement = 0.1f;
float VNode::MaxWidth = 2.0f;