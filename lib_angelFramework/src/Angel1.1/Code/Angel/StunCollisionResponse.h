#pragma once

#include "CollisionResponse.h"

class StunCollisionResponse : public CollisionResponse
{
public:
	DECLARE_COLLISIONRESPONSE(StunCollisionResponse)

	virtual void Execute();

};