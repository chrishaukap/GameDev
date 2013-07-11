#pragma once

class BackgroundActor : public Actor
{
public:
   BackgroundActor();

   void Render();
   void UpdateUVs(float horizontal, float vertical);
   void ScrollLeft(float delta);
   void ScrollRight(float delta);
   void ScrollUp(float delta);
   void ScrollDown(float delta);
};



