#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector3d.h"

class Particle
{
public:
   void integrate(float duration);

   bool hasFiniteMass() const;
   void clearAccumulator();
   void addForce(const Vector3 &force);

   // getters
   float mass() const;
   float inverseMass() const;
   float damping() const;
   virtual const Vector3& position() const;
   virtual const Vector3& velocity() const;
   const Vector3& acceleration() const;

   // setters
   void setMass(const float mass);
   void setInverseMass(const float inverseMass);
   void setDamping(const float damping);
   virtual void setPosition(const Vector3 &position);
   virtual void setPosition(const float x, const float y, const float z);
   virtual void setVelocity(const Vector3 &velocity);
   virtual void setVelocity(const float x, const float y, const float z);
   void setAcceleration(const Vector3 &acceleration);
   void setAcceleration(const float x, const float y, const float z);

protected:
   float m_inverseMass;
   float m_damping;
   Vector3 m_position;
   Vector3 m_velocity;
   Vector3 m_forceAccum;
   Vector3 m_acceleration;
};

#endif
