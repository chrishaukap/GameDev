#pragma once

#include "Actors/Actor.h"

namespace Edge
{
	class TimHead : public Actor
	{
	public:
		TimHead();
		~TimHead();

		void Update(float dt);
	private:
	};
}