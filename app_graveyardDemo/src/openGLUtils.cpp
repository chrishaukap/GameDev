#ifdef _WINDOWS
#include <windows.h>
#endif

#include "openGLUtils.h"
#include "defines.h"
#include <math.h>

#include <gl/gl.h>

void
OpenGLUtils::glDrawFilledCircle(CHUint numEdges, myglColor &color )
{   
   float angle;
   glBegin( GL_POLYGON );
   for( CHUint i=0; i < numEdges; i++)
   {
      angle = i * 2 * PI/numEdges;
      glVertex3f( cos(angle), sin(angle), 0.0f );
   }
   glEnd();
}
void
OpenGLUtils::glDrawCircle(CHUint numEdges)
{   
   float angle;
   glBegin( GL_POLYGON );
   for( CHUint i=0; i < numEdges; i++)
   {
      angle = i * 2 * PI/numEdges;
      glVertex3f( cos(angle), sin(angle), 0.0f );
   }
   glEnd();
}