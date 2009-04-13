#include "blood/bloodDrop.h"

void BloodDrop::update()
{
   ++duration;
   position = utils3D::translatePointAlongVector(position, orientation, speed);
}