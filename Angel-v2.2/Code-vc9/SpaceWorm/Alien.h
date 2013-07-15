#pragma once

class Alien : public Actor
{
public:
	Alien();

   void Update(float dt);

private:
   Vector2 m_orientation;
   float m_wormeSpentMoving;
   float m_wormeToSpendMoving;
   float m_speed;
};
