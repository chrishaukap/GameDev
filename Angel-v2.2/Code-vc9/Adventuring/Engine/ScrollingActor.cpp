#include "stdafx.h"
#include "ScrollingActor.h"


BackgroundActor::BackgroundActor(){}

void BackgroundActor::Render()
{
   Actor::Render();
}
void BackgroundActor::UpdateUVs(float horizontal, float vertical)
{
   Vector2 lowerLeft;
   Vector2 upperRight;
   GetUVs(lowerLeft, upperRight);

   lowerLeft.X += horizontal;
   lowerLeft.Y += vertical;
   upperRight.X += horizontal;
   upperRight.Y += vertical;

   SetUVs(lowerLeft, upperRight);      
}
void BackgroundActor::ScrollLeft(float delta)
{
   UpdateUVs(-delta,0);
}
void BackgroundActor::ScrollRight(float delta)
{
   UpdateUVs(delta, 0);
}
void BackgroundActor::ScrollUp(float delta)
{
   UpdateUVs(0, delta);
}
void BackgroundActor::ScrollDown(float delta)
{
   UpdateUVs(0, -delta);
}


