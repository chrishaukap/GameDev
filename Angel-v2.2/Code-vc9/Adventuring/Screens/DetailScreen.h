#pragma once

#include "GameScreen.h"

class DetailScreen : public GameScreen
{
public:
	DetailScreen(unsigned int screenWidth, unsigned int screenHeight);
   ~DetailScreen();


protected:
   
	virtual void doStart();
   virtual void doStop();
   virtual void doPause();
   virtual void doResume();
	virtual void doUpdate(float dt);
private:
   void RenderBackGround();
   void RenderCombatScene();
   void UpdateDebug();
   unsigned int m_screenWidth;
   unsigned int m_screenHeight;

   std::vector<Actor*> m_actors;
};
