#pragma once

#include "Renderable.h"

class GridActor : public Renderable
{
public:
	GridActor();
	GridActor(Color3f lines, Color3f axis, float interval);
	virtual ~GridActor(){}

	void SetLineColor(Color3f lineCol);
	const Color3f GetLineColor();

	void SetAxisColor(Color3f axisCol);
	const Color3f GetAxisColor();

	void SetInterval(float interval);
	const float GetInterval();

	virtual void DrawAxes();
	virtual void Render();
	virtual void Update(float dt) {}

protected:
	Color3f _lineColor;
	Color3f _axisColor;
	float _interval;
	Vector2 _minCoord;
	Vector2 _maxCoord;
private:
};