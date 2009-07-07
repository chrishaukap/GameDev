#pragma once

#include "CollisionResponse.h"

struct Sound;

class PlaySoundCollisionResponse : public CollisionResponse
{
public:
	DECLARE_COLLISIONRESPONSE(PlaySoundCollisionResponse)

	virtual void Execute();
protected:
	Sound*		_sound;
	float		_volume;
};