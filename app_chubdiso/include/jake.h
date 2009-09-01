#pragma once

#include "cdhBase.h"
#include "MouseInput.h"
#include "VecStructs.h"
#include "iCollidable.h"
#include "Actor.h"

namespace CDH
{
	namespace Chubdiso
	{
      class Jake : public Actor, public MouseListener
		{
		public:
			Jake();
			~Jake();

         void Init();
         void Clear();
			void Update(float dt);

	      // inherited from MouseListener
	      void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
	      void MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button);
	      void MouseMotionEvent(Vec2i screenCoordinates);
			
         IMPLEMENT_INDEXABLE_CLASS(Jake)
		private:
			float m_speed;
		};
	}
}