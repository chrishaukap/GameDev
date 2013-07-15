#pragma once

#include "iCollidable.h"

class CollidingParticle : public Particle, public iCollidable
{
public:
	CollidingParticle();
   ~CollidingParticle(){}

   void Hit();

private:
};