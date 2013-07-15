#pragma once

#include "cdhBase.h"

struct Vector2;
namespace CDH
{
	namespace Edge
	{
		class TimHead;
		class TimTail
		{
		public:
			TimTail(const TimHead& head);
			~TimTail();

         void moveTo(const Vector2& point, float distance);
         void addTailNode();
		private:
			const TimHead& m_head;
         class Data;
         Data* m_data;
		};
	}
}