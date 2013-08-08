#pragma once

#include "cdhBase.h"

namespace Edge
{
   class iCollidable;
	void CollisionIndieIndie(iCollidable& indie1, iCollidable& indie2);
	void CollisionTimIndie(iCollidable& tim, iCollidable& indie);
	void CollisionMoneyIndie(iCollidable& money, iCollidable& indie);
}