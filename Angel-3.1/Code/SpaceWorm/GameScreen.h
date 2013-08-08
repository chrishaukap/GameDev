#pragma once

#include "DemoGameManager.h"
#define theGame GameScreen::GetInstance()

class Worm;
class GameScreen : public MouseListener, public DemoScreen
{
public:
	static GameScreen &GetInstance();

	virtual void Start();
   virtual void Stop();
   virtual void Update(float dt);
   virtual void Cleanup();
   Worm& worm();
   const Worm& worm()const;


   void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
   void MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button);
private:
   // determines if the coordinates are in the bounds of one of the planets.
   // if so, planet is set to the planet that was clicked on, if not it is NULL
   bool ClickedOnPlanet(const Vec2i& screenCoords, class Planet*& planet) const;
   void UpdatePlanetSelections(class Planet* planet);

	GameScreen();
	static GameScreen *s_GameScreen;
	class StarSystem* m_starSystem;
   FullScreenActor* m_background;
   Worm* m_worm;
};
