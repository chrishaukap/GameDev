
#include <assert.h>
#include <particle.h>
#include <math.h>
#include <float.h>

void Particle::integrate(float duration)
{
   // We don't integrate things with zero mass.
   if (m_inverseMass <= 0.0f) 
      return;

   assert(duration > 0.0);

   // Update linear position.
   m_position.addScaledVector(m_velocity, duration);

   // Work out the acceleration from the force
   Vector3 resultingAcc = m_acceleration;
   resultingAcc.addScaledVector(m_forceAccum, m_inverseMass);

   // Update linear velocity from the acceleration.
   m_velocity.addScaledVector(resultingAcc, duration);

   // Impose drag.
   m_velocity *= powf(m_damping, duration);

   // Clear the forces.
   clearAccumulator();
}

void Particle::setMass(const float mass)
{
   assert(mass != 0);
   m_inverseMass = ((float)1.0)/mass;
}

float Particle::mass() const
{
   if (m_inverseMass == 0)
      return FLT_MAX;
   else 
      return ((float)1.0)/m_inverseMass;
}

void Particle::setInverseMass(const float inverseMass)
{
   m_inverseMass = inverseMass;
}

float Particle::inverseMass() const
{
   return m_inverseMass;
}

bool Particle::hasFiniteMass() const
{
   return m_inverseMass >= 0.0f;
}

void Particle::setDamping(const float damping)
{
   m_damping = damping;
}

float Particle::damping() const
{
   return m_damping;
}

void Particle::setPosition(const Vector3 &position)
{
   m_position = position;
}

void Particle::setPosition(const float x, const float y, const float z)
{
   m_position.x = x;
   m_position.y = y;
   m_position.z = z;
}

const Vector3& 
Particle::position() const
{
   return m_position;
}

void Particle::setVelocity(const Vector3 &velocity)
{
   m_velocity = velocity;
}

void Particle::setVelocity(const float x, const float y, const float z)
{
   m_velocity.x = x;
   m_velocity.y = y;
   m_velocity.z = z;
}


const Vector3& 
Particle::velocity() const
{
   return m_velocity;
}

void Particle::setAcceleration(const Vector3 &acceleration)
{
   m_acceleration = acceleration;
}

void Particle::setAcceleration(const float x, const float y, const float z)
{
   m_acceleration.x = x;
   m_acceleration.y = y;
   m_acceleration.z = z;
}


const Vector3& 
Particle::acceleration() const
{
   return m_acceleration;
}

void Particle::clearAccumulator()
{
   m_forceAccum.clear();
}
void Particle::addForce(const Vector3 &force)
{
   m_forceAccum += force;
}



