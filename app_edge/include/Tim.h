#pragma once

#include "cdhBase.h"
#include "MouseInput.h"
#include "Vector2.h"
#include "VecStructs.h"
#include "iCollidable.h"

namespace CDH
{
	namespace Edge
	{
      class TimHead;
      class TimTail;
      class Tim : public MouseListener, public iCollidable
		{
		public:
			Tim();
			~Tim();

         void Init();
         void Clear();
			void Update(float dt);
			
			const Vector2 GetPosition();
			const Vector2 GetSize();

         void GrowTail();

	      // inherited from MouseListener
	      void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
	      void MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button);
	      void MouseMotionEvent(Vec2i screenCoordinates);

         IMPLEMENT_INDEXABLE_CLASS(Tim)
		private:
         TimHead* m_head;
         TimTail* m_tail;
         float m_speed;
		};
	}
}