#pragma once

#include "iCollidable.h"
#include "MyParticleActor.h"

class CollidingParticle : public Particle, public iCollidable
{
public:
	CollidingParticle();
   ~CollidingParticle(){}

   void Hit();

private:
};