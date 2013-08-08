#include "stdafx.h"
#include "collisions.h"
#include "CollidingParticle.h"
#include "Engine/Worm.h"
#include "Planet.h"

#include <assert.h>

void CollisionParticlePlanet(iCollidable& particle, iCollidable& planet)
{
   CollidingParticle* pParticle = dynamic_cast<CollidingParticle*>(&particle);
   pParticle->Hit();

   Planet* pPlanet = dynamic_cast<Planet*>(&planet);
   pPlanet->Hit();
}
void CollisionParticleWorm(iCollidable& particle, iCollidable& worm)
{
   CollidingParticle* pParticle = dynamic_cast<CollidingParticle*>(&particle);
   pParticle->Hit();

   Worm* pWorm = dynamic_cast<Worm*>(&worm);
   pWorm->GrowTail();
}
