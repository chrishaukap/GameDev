#include "cdhBase.h"
#include "sprite/renderable.h"
#include "sprite/sprite.h"
#include <stdio.h>

using namespace CDH;

Renderable::Renderable() : 
   m_sprite(NULL)
{   
	m_sprite = new Sprite();
}
Renderable::~Renderable()
{
   delete m_sprite;
}

Sprite*
Renderable::sprite() const
{
   return m_sprite;
}