#pragma once

#include "Actor.h"
#include "MouseInput.h"

class Cell : public Actor, public MouseListener
{
public: 
	void MouseDownEvent(Vec2i screenCoords, MouseButtonInput button);
private:
};