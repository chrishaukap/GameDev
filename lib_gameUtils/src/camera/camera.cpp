#include "camera/camera.h"
#include "cdhBase.h"
#include <math.h>

using namespace CDH;

const float Camera::yMax = 10.0f;
const float Camera::yMin = -yMax;

Camera::Camera(const Vector3& v) :
   dist(v.z), x(v.x), y(v.y), z(dist), yaw(PIOver2), 
   spd(0.0125f), orbiting(false), yGoingUp(false), yOscillating(false)
{         
}


void Camera::computePosition()
{
   x = cos(yaw) * dist;
   z = sin(yaw) * dist;
}

void Camera::zoomOut()
{
   dist += 1;
   computePosition();
}
void Camera::zoomIn()
{
   dist -= 1;
   computePosition();
}

void Camera::up()
{
   y += 1;
   
}
void Camera::down()
{
   y -= 1;
}

void Camera::moveLateralDistance( float distXAxis )
{
   /*yaw += PIOver256 * distXAxis;
   if( (yaw += spd) > TWOPI)
      yaw = 0.0f;
   computePosition();*/
}
void Camera::process()
{
   if(yOscillating)
   {
      static const float dY = 0.5f;
      if(yGoingUp)
      {
         y += dY;
         if(y >= yMax)
         {
            y = yMax;
            yGoingUp = false;
         }
      }
      else
      {
         y -= dY;
         if( y <= yMin )
         {
            y = yMin;
            yGoingUp = true;
         }
      }
   }
   if(orbiting)
   {      
      if( (yaw += spd) > TWOPI)
         yaw = 0.0f;
      computePosition();
   }
}

