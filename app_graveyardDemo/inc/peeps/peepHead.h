#ifndef PEEPHEAD_H
#define PEEPHEAD_H

#include "abstractPeep.h"

class PeepHead : public AbstractPeep
{
public:
   PeepHead(const Vector3& position, 
            const Vector3& orientation, 
            const Vector3& rotation,
            float scale);
   ~PeepHead();

   void setScale(float scale) {m_scale = scale;}
   float scale() const {return m_scale;}
   void setMouthScale(float scale) {m_mouthScale = scale;}
   void setEyeScale(float scale) {m_eyeScale = scale;}
   
   // inherited from MoveableObject
   BoundingCube getBoundingCube() const;

   // inherited from AbstractPeep
   PeepType peepType() const {return TypePeepHead;}
   void renderDead() const;
   void render() const;

private:
   void renderFace() const;
   void renderMouth() const;
   void renderDeadEyes() const;
   void renderEyes() const;
   float m_scale, m_mouthScale, m_eyeScale;
};

#endif // PEEPHEAD_H