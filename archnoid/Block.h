#pragma once

#include "defs.h"
#include "defaults.h"
#include "Ball.h"

namespace Archnoid
{

class Block
{
public:
   Block(USHORT x, USHORT y, 
         USHORT w= BlockDefaults::WIDTH,
         USHORT h= BlockDefaults::HEIGHT,
         USHORT hp= BlockDefaults::HP,
         float sp= BlockDefaults::SPEED);
   ~Block(void);

   // getters
   USHORT GetX() const {return m_x;}
   USHORT GetY() const {return m_y;}
   USHORT GetWidth() const {return m_width;}
   USHORT GetHeight() const {return m_height;}
   USHORT GetHitPoints() const {return m_hitpoints;}
   float GetSpeed() const {return m_speed;}
   SHORT GetVectorX() const {return m_vectX;}
   SHORT GetVectorY() const {return m_vectY;}
   USHORT DecrementHP();
   bool isDestroyed() const {return m_destroyed;}



   // setters
   void SetSpeed(float s) {m_speed = s;}
   void SetVectorX(SHORT x) {m_vectX = x;}
   void SetVectorY(SHORT y) {m_vectY = y;}
   void SetX(USHORT x) {m_x = x;}
   void SetY(USHORT y) {m_y = y;}
   void SetWidth(USHORT w) {m_width = w;}
   void SetHeight(USHORT h) {m_height = h;}
   void SetHitPoints(USHORT hp) {m_hitpoints = hp;}
   
   void Move();
   CollisionType IsCollision(const Ball &ball) const;   

private:
   USHORT m_x;
   USHORT m_y;
   USHORT m_width;
   USHORT m_height;
   USHORT m_hitpoints;
   USHORT m_visible;
   bool m_destroyed;
   
   float m_speed;
   SHORT m_vectX;
   SHORT m_vectY;
};


} // end namespace