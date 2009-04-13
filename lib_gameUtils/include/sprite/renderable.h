#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "cdhBase.h"

namespace CDH
{

class Sprite;
class Animation;
class Renderable
{
public:
   Renderable();  
   virtual ~Renderable();

   virtual Status render() const = 0;
   Sprite* sprite() const;
   
protected:
   Sprite* m_sprite;

private:   
   Renderable(const Renderable& C);
   const Renderable& operator=(const Renderable& C);
};


}
#endif
