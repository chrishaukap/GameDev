#pragma once

#include "GameScreen.h"

class Vine;
class VineScreen : public GameScreen
{
public:
   VineScreen(unsigned int width, unsigned int height );
   ~VineScreen();

   virtual void Start();
	virtual void Update(float dt);
	virtual void Render() ;
	virtual void Stop();
private:
   VineScreen();
	Vine* createVine();

   unsigned int m_screenWidth, m_screenHeight;
	Vine* m_vine;
};

