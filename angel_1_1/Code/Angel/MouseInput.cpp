#include "stdafx.h"
#include "MouseInput.h"

std::set<MouseListener*> _mouseSubscribers;

MouseListener::MouseListener()
{
	RegisterMouseSubscriber(this);
}

MouseListener::~MouseListener()
{
	DeregisterMouseSubscriber(this);
}


void MouseListener::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button) {}
void MouseListener::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button) {}
void MouseListener::MouseMotionEvent(Vec2i screenCoordinates) {}


void RegisterMouseSubscriber(MouseListener* actor)
{
	_mouseSubscribers.insert(actor);
}

void DeregisterMouseSubscriber(MouseListener *actor)
{
	std::set<MouseListener*>::iterator it = _mouseSubscribers.find(actor);
	if (it != _mouseSubscribers.end())
	{
		_mouseSubscribers.erase(it);
	}
}


void MouseMotion(int x, int y)
{
	std::set<MouseListener*>::iterator it = _mouseSubscribers.begin();
	while (it != _mouseSubscribers.end())
	{
		(*it)->MouseMotionEvent(Vec2i(x, y));
		it++;
	}
}

void MouseButton(int button, int state, int x, int y)
{
	MouseButtonInput mouseButton;
	if (button == GLUT_RIGHT_BUTTON)
	{
		mouseButton = MOUSE_RIGHT;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		mouseButton = MOUSE_MIDDLE;
	}
	else
	{
		mouseButton = MOUSE_LEFT;
	}

	std::set<MouseListener*>::iterator it = _mouseSubscribers.begin();
	while (it != _mouseSubscribers.end())
	{
		if (state == GLUT_DOWN)
		{
			(*it)->MouseDownEvent(Vec2i(x, y), mouseButton);
		}
		else
		{
			(*it)->MouseUpEvent(Vec2i(x, y), mouseButton);
		}
		it++;
	}
}
