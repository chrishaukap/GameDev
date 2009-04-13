#pragma once

#include "MouseInput.h"

class TextActor;
class Actor;
class LifeChoice : public MouseListener
{
public:
	LifeChoice(float x, float y, // center of icon piece for now...
				  float iconWidth, float iconHeight,
				  float bttnSize, 
				  Color3f color = Color3f(0,0,0),
				  String string = String());

   void AddToWorld();
   void RemoveFromWorld();
	void SetPosition(float x, float y);
	unsigned int GetPoints() const;
	void SetPoints(unsigned int points);
   void finalizePoints();
   
	// inherited from MouseListener
	void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
	void MouseMotionEvent(Vec2i screenCoordinates);

private:
	float m_x, m_y;
	unsigned int m_tmpPoints;
   unsigned int m_points;
   Actor* m_icon;
	TextActor* m_iconText;
	Actor* m_numPoints;
   TextActor* m_numPointsText;
   Actor* m_addPoint;
   Actor* m_subPoint;

	static const float m_actorPadding;

	void addPoints();
	void decPoints();
	void SetNumPointsDisplayString();

};