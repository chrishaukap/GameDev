#pragma once

#include "iCollidable.h"
#include "Actors/Actor.h"

class Planet : public Actor, 
               public iCollidable
{
public:
	Planet();

   bool IsSelected() const {return m_selected;}
   void Select(bool select) {m_selected = select;}
   void Hit();
   void Update(float dt);
 
private:
   float m_growthCounter;
   bool m_selected;
};
