#pragma once

#include <vector>

class VNode;
typedef std::vector<VNode*> VINENODES;
class VNode
{
public:
	VNode(const Vector2& location, const Vector2& orientation);
	~VNode();
	void grow(const Vector2& targetPoint, bool& reachedTarget,
				 VINENODES& newChildren, VINENODES& staleChildren);
	void render() const;
private:
	void addRandomizedChildren(const Vector2& startPoint, const Vector2& targetPoint);
   
   inline const Vector2&
   VNode::endPoint() const
   {
	   return m_endPoint;
   }
   inline void
   VNode::setEndPoint()
   {
      m_endPoint = m_location + (m_orientation * m_length);
   }
   inline void 
   VNode::incrementGrowth()
   {
	   m_length += GrowthIncrement;
   }
   inline bool 
   VNode::maxLengthReached()
   {		
	   return bool(m_length >= MaxLength);
   }

	static float MaxLength;
	static float GrowthIncrement;
   static float MaxWidth;
	Vector2 m_location;
   Vector2 m_endPoint;
	Vector2 m_orientation;
	float m_length;
	std::vector<VNode*> m_children;
};