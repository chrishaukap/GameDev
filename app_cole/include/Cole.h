#pragma once

#include "cdhBase.h"
#include "MouseInput.h"
#include "PhysicsActor.h"
#include "VecStructs.h"
#include "iCollidable.h"

namespace CDH
{
	namespace Cole
	{
      class ColeAvatar : public PhysicsActor, public iCollidable
		{
		public:
			ColeAvatar();
			~ColeAvatar();

         void Init();
         void Clear();
			void Update(float dt);

			void jump();
			void moveLeft();
			void moveRight();
			
         IMPLEMENT_INDEXABLE_CLASS(Cole)
		private:
			float m_speed;			
		};
	}
}