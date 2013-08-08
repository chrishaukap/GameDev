#pragma once
#include "cdhBase.h"

struct Vector2;

class WormHead;
class WormTail
{
public:
	WormTail(const WormHead& head, int maxLength);
	~WormTail();

   void AddToWorld();
   void moveTo(const Vector2& point, float distance);
   void addTailNode();
   bool IntersectsWithRect( const struct Rect<float>& ) const;
private:
	const WormHead& m_head;
   class Data;
   Data* m_data;
   int m_maxLength;
};