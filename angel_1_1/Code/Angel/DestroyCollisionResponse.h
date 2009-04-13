#pragma once

#include "CollisionResponse.h"

class DestroyCollisionResponse : public CollisionResponse
{
public:
	DECLARE_COLLISIONRESPONSE(DestroyCollisionResponse)

	virtual void Execute();

};