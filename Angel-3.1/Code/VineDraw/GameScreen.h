#pragma once

#include "Infrastructure/Renderable.h"
#include <vector>

class GameScreen : public Renderable
{
public:
	GameScreen(){}

   virtual void Start();
	virtual void Stop();

	virtual void Update(float dt){}
	virtual void Render(){}

protected:
	std::vector<Renderable*> m_objects;
};
