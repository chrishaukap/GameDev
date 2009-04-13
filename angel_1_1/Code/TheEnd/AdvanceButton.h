#pragma once

#include "MouseInput.h"
#include "Actor.h"

#define theAdvanceButton AdvanceButton::GetInstance()

class TextActor;
class AdvanceButton : public Actor, public MouseListener
{
public:
   static AdvanceButton& GetInstance();
	AdvanceButton();

   void AddToWorld();
   void RemoveFromWorld();

	// inherited from MouseListener
	void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
	void MouseMotionEvent(Vec2i screenCoordinates);

   // inherited from Action
   void Update(float dt);
	virtual void SetPosition(float x, float y);

private:
	static AdvanceButton* s_advanceButton;
   TextActor* m_text;
};