#ifndef BALL_H
#define BALL_H

#include "defs.h"
#include "defaults.h"
#include <vector>
using namespace std;

namespace Archnoid
{

// BALL
class Ball
{
public:
   Ball(USHORT rad, SHORT cntr_x, SHORT cntr_y, float speed, SHORT vecX, SHORT vecY):
      m_radius(rad),m_cntrX(cntr_x),m_cntrY(cntr_y),m_speed(speed),m_vectX(vecX),m_vectY(vecY) {}
   ~Ball(){}
   
   // GETTERS
   SHORT GetULX() const {return m_cntrX - m_radius;}
   SHORT GetULY() const {return m_cntrY - m_radius;}
   SHORT GetLRX() const {return m_cntrX + m_radius;}
   SHORT GetLRY() const {return m_cntrY + m_radius;}
   float GetVectX() const {return m_vectX;}
   float GetVectY() const {return m_vectY;}
   SHORT GetCntrX() const {return m_cntrX;}
   SHORT GetCntrY() const {return m_cntrY;}
   float GetSpeed() const {return m_speed;}

   void UnMove();
   void Move();

   // resolves collision with horizontal or vertical surface.
   void ResolveCollision( SurfaceType surface );

private:
   USHORT m_radius;
   float m_speed;
   SHORT m_cntrX;
   SHORT m_cntrY;
   SHORT m_vectX;
   SHORT m_vectY;
};

class Balls
{
public:
   Balls() : m_balls(), m_ballIter(0) {}
   ~Balls() 
   {
      size_t sz = m_balls.size();
      for( size_t i=0; i<sz; ++i )
         delete m_balls[i];
   }
   void TakeBall(Ball* ball) {m_balls.push_back(ball);}

   Ball* GetFirstBall() const ;
   Ball* GetNextBall() const;

private:
   // HAUKAP - should be a list for deletion of balls
   typedef std::vector<Ball*> BALL_LIST;
   BALL_LIST m_balls;
   mutable USHORT m_ballIter; // HAUKAP -  should be an iterator
};



} // end namespace

#endif // BALL_H