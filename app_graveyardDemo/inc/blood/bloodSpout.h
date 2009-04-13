#ifndef BLOODSPOUT_H
#define BLOODSPOUT_H

#include "vector3d.h"
#include <vector>

enum BloodSpoutDensity
{
   Sparse,Normal,Dense
};

class BloodDrop;
typedef std::vector<BloodDrop*> DROPLETS;

class BloodSpout
{
public:
   BloodSpout(const Vector3& position,
              const Vector3& orientation,
              const Vector3& rotation,
              BloodSpoutDensity density,
              float speed,
              float size,
              float duration) :
      m_position(position), 
      m_orientation(orientation),
      m_rotation(rotation),
      m_density(density),
      m_dropletSpeed(speed), 
      m_dropletSize(size),
      m_dropletDuration(duration),
      m_droplets(),
      m_shouldStop(false)
      {}
   ~BloodSpout();

   void render() const;
   void process();

   // direction blood spurts
   const Vector3& rotation() const {return m_rotation;}

   // direction spout as a hole is moving
   const Vector3& orientation() const {return m_orientation;}
   const Vector3& position() const {return m_position;}
   void setPosition(const Vector3& v);
   void setRotation(const Vector3& v) {m_rotation = v;}
   void setOrientation(const Vector3& v) {m_orientation = v;}

   bool stopped() const {return m_shouldStop;}
   void stop() {m_shouldStop = true;}

   // DEBUG
   CHUint numDroplets() const;
   // members
private:
   Vector3 m_position;
   Vector3 m_orientation;
   Vector3 m_rotation;
   BloodSpoutDensity m_density;
   float m_dropletSpeed;
   float m_dropletSize;
   float m_dropletDuration;
   DROPLETS m_droplets;   
   bool m_shouldStop;
};


#endif // BLOODSPOUT_H