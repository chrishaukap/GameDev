#ifndef PEEP_H
#define PEEP_H

#include "cdhbase.h"
#include "vector3D.h"
#include <vector>
#include "abstractPeep.h"

using namespace CDH;

class PeepHead;
class PeepBody;
class Peep : public AbstractPeep
{
public:
   Peep(const Vector3& position, 
        const Vector3& orientation, 
        const Vector3& rotation,
        float scale );
   ~Peep();
   
   float scale() const;
   void setScale(float scale);
   void Split(PeepHead*& head, PeepBody*& body);
   void move();   

   // inherited from MoveableObject
   void setOrientation(const Vector3& orientation);
   const Vector3& orientation() const;
   void setPosition(const Vector3& position);
   const Vector3& position() const;
   BoundingCube getBoundingCube() const;

   // inherited from AbstractPeep
   PeepType peepType() const {return TypePeep;}
   void renderDead() const;
   void render() const;

   void animate();
private:
   Peep();   
   Peep(const Peep& T);
   const Peep& operator=(const Peep& T);

   void getAnimFrameIdx(CHUint &maskIdx, CHUint &txtIdx) const;

   void flipZOrientation();
   void flipXOrientation();
   void RenderBoundingCube() const;

   
   CHUint m_animCounter;
   CHUint m_animFrameIdx;

   bool m_ownsParts;
   PeepHead* m_head;
   PeepBody* m_body;

   enum PeepType2;
   PeepType2 m_type;
};

#endif // PEEP_H