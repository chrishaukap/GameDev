#ifndef OPENGLUTILS_H
#define OPENGLUTILS_H

#include "cdhBase.h"
#include "defines.h"

namespace CDH
{

struct myglColor
{
   myglColor(float R, float G, float B) : r(R), g(G), b(B) {}
   float r,g,b;
};

class OpenGLUtils
{
public:
   static void glDrawFilledCircle(CHUint numEdges, myglColor &color );
   static void glDrawCircle(CHUint numEdges);
private:
   OpenGLUtils();
};
}

#endif //OPENGLUTILS_H