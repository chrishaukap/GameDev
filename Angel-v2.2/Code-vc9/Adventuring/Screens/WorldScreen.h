#pragma once

#include "GameScreen.h"

class WorldScreen : public GameScreen
{
public:
	WorldScreen(unsigned int screenWidth, unsigned int screenHeight);
   ~WorldScreen();

protected:
	virtual void doStart();
   virtual void doStop();
   virtual void doPause();
   virtual void doResume();
	virtual void doUpdate(float dt);

private:
   void UpdateDebug();
   unsigned int m_screenWidth;
   unsigned int m_screenHeight;
	class BackgroundActor *m_backgroundMap;      
};
