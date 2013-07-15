#include "stdafx.h"
#include "cdhBase.h"
#include "Planet.h"
#include "GameScreen.h"

static const int NumPlanetGraphics = 3;
std::string g_planetGfxString[] = 
   {
      "Resources/Images/planets/Earth_01.png",
      "Resources/Images/planets/face_01.png",
      "Resources/Images/planets/humanplanet_01.png"
   };

Planet::Planet() : 
   m_growthCounter(0.0f), 
   m_selected(false)
{
   SetColor(1,1,1,1);
   SetSize(2);
   SetPosition(MathUtil::RandomFloatInRange(-10, 10),
               MathUtil::RandomFloatInRange(-10, 10));
   int rand = MathUtil::RandomInt(NumPlanetGraphics);
	LoadSpriteFrames(g_planetGfxString[rand]);
	PlaySpriteAnimation(0.5f, SAT_Loop, 0, 2);
}
static float s_sizeChangePercentage = 0.05f;
void Planet::Hit()
{
   SetSize( GetSize().X - GetSize().X * s_sizeChangePercentage);
   
   SetColor(1,0,0,1);
}

static float s_growthInterval = 2.0f;
void Planet::Update(float dt)
{
   m_growthCounter += dt ;
   if(m_growthCounter >= s_growthInterval)
   {
      SetColor(1,1,1,1);
      m_growthCounter = 0;
      SetSize( GetSize().X + GetSize().X * s_sizeChangePercentage );
   }
   Actor::Update(dt);
}