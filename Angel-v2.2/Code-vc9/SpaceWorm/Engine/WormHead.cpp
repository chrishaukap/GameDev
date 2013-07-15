#include "stdafx.h"
#include "Engine/WormHead.h"

WormHead::WormHead() 
{
   theWorld.Add(this,10);
   SetRotation(0);
   SetSprite("Resources/Images/WormHead.png");
}
WormHead::~WormHead()
{}

void WormHead::Update(float dt)
{
}