#pragma once

#include "cdhBase.h"

struct Vector2;
namespace CDH
{
	namespace Edge
	{
		class TimTail
		{
		public:
			TimTail();
			~TimTail();

         void moveTo(const Vector2& point, float distance);
         void addTailNode();
		private:
         class Data;
         Data* m_data;
		};
	}
}