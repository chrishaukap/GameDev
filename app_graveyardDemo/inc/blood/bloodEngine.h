#ifndef BLOODENGINE_H
#define BLOODENGINE_H

#include "cdhBase.h"
#include "vector3d.h"
#include <vector>
using namespace CDH;

class BloodDrop;
class BloodSpout;
enum BloodSpoutDensity;

class Vector3;
class BloodEngine
{
public:
   BloodEngine();   
   ~BloodEngine();

   void render() const;
   void process();

   // DEBUG
   CHUint numSpouts() const;
   CHUint numDroplets() const;

   void addDroplet(const Vector3& position,
                   const Vector3& orientation,
                   float speed,
                   float size,
                   float duration);
   BloodSpout* 
      addSpout(const Vector3& position,
               const Vector3& orientation,
               const Vector3& rotation,
               BloodSpoutDensity density, /*max number of individual droplets*/
               float speed, /* max speed of the projectile droplets */
               float size, /* size of individual droplets */
               float duration /* distance the droplets will travel before deletion */ );

private:
   
   std::vector<BloodDrop*> m_droplets;
   std::vector<BloodSpout*> m_spouts;
};
typedef std::vector<BloodDrop*> DROPLETS;

#endif // BLOODENGINE_H