#ifndef PEEPBODY_H
#define PEEPBODY_H

#include "abstractPeep.h"

class PeepBody : public AbstractPeep
{
public:
   PeepBody(const Vector3& position, 
            const Vector3& orientation,
            const Vector3& rotation,
            float scale);
   ~PeepBody();
   
   // inherited from MoveableObject
   BoundingCube getBoundingCube() const;

   
   // inherited from AbstractPeep
   PeepType peepType() const {return TypePeepBody;}
   void renderDead() const;
   void render() const;

private:
   float m_leftArmAngleY, m_rightArmAngleY;
   float m_leftLegAngleY, m_rightLegAngleY;
   float m_scale;
};

#endif // PEEPBODY_H