#include "stdafx.h"

#include "Engine/Monster.h"
#include "Engine/GameWorld.h"

Monster::Monster() : 
   Combatant("Monster")
{
   m_direction = MathUtil::RandomVector(Vector2(-1,-1),Vector2(1,1));
   m_direction.Normalize();
   m_health = 2;
}

Monster::~Monster() 
{
}

void Monster::UpdateCombatant(float dt)
{  
#if 1
   float chance = MathUtil::RandomFloat(100.0f);
   if(chance >= 95)
   {   
      m_direction = MathUtil::RandomVector(Vector2(-1,-1),Vector2(1,1));
      m_direction.Normalize();   
   }

   Vector2 monsterpos = GetPosition();
   const float speed = 1.0f;
   monsterpos.X += m_direction.X * dt * speed;
   monsterpos.Y += m_direction.Y * dt * speed;
   SetPosition(monsterpos);
#endif
   Actor::Update(dt);
}

bool Monster::shouldEngage(const class Hero& hero)
{
   if(inCombat())
      return false;
   return true;
}