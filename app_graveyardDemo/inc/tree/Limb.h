#ifndef LIMB_H
#define LIMB_H

#include "cdhBase.h"
using namespace CDH;

#include "defines.h"
#include <vector>

class Vector3;
class MoveableObject;
class LimbNode;
class Limb
{
public:
   enum LimbState
   {
      LimbFree,
      LimbPursuing,
      LimbThrashing,
   };
public:
   Limb(const Vector3& position, const Vector3& orientation);
   ~Limb();

   // grows toward the input point (according to our growthrate)
   void grow();
   void move();

   void growBranches();
   bool targetAquired() const;
   void removeTarget();
   bool isFree() const { return m_state == LimbFree;}
   bool isPursuing() const { return m_state == LimbPursuing; }
   bool isThrashing() const { return m_state == LimbThrashing; }
   void assignTarget( MoveableObject& target );

   void thrash();
   void render() const;

private:
   typedef std::vector<LimbNode*> LIMBNODES;
   Limb();
   Limb(const Limb& T);
   const Limb& operator=(const Limb& T);
   
   // returns the vector from the tip of the limb in its starting position
   // to the tip of the limb in its ending position
   Vector3 propogateThrash();
   void increaseWidth();
   float totalLimbLength() const;
   LimbNode* findFurthestParentNeededToRotate(const MoveableObject& pursuitObject) const;

   LIMBNODES m_nodes;
   MoveableObject* m_pursuitObject;
   CHUint m_maxLimbNodes;
   LimbState m_state;
   float m_curThrashAngle;
};



#endif // LIMB_H