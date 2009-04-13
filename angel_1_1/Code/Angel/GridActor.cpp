#include "stdafx.h"
#include "GridActor.h"

GridActor::GridActor()
{
	//yay for magic numbers! (default parameters of the grid)
	_lineColor = Color3f(.76f, .83f, 1.0f);
	_axisColor = Color3f(1.0f, .41f, .6f);
	_interval = 1.0f;
	_minCoord = Vector2(-100.0f, -100.0f);
	_maxCoord = Vector2(100.0f, 100.0f);
}

GridActor::GridActor(Color3f lines, Color3f axis, float interval)
{
	_lineColor = lines;
	_axisColor = axis;
	_interval = interval;
	_minCoord = Vector2(-100.0f, -100.0f);
	_maxCoord = Vector2(100.0f, 100.0f);
}

void GridActor::SetLineColor(Color3f lineCol)
{
	_lineColor = lineCol;
}

const Color3f GridActor::GetLineColor()
{
	return _lineColor;
}

void GridActor::SetAxisColor(Color3f axisCol)
{
	_axisColor = axisCol;
}

const Color3f GridActor::GetAxisColor()
{
	return _axisColor;
}

void GridActor::SetInterval(float interval)
{
	_interval = interval;
}

const float GridActor::GetInterval()
{
	return _interval;
}

void GridActor::Render()
{
	glColor3f(_lineColor.R, _lineColor.G, _lineColor.B);
	glBegin(GL_LINES);
	float i;
	for(i = _minCoord.X; i <= _maxCoord.X; i += _interval)
	{
		glVertex2f(i, _minCoord.Y);
		glVertex2f(i, _maxCoord.Y);
	}
	for(i = _minCoord.Y; i <= _maxCoord.Y; i += _interval)
	{
		glVertex2f(_minCoord.X, i);
		glVertex2f(_maxCoord.X, i);
	}
	
	DrawAxes();
	glEnd();
}
void GridActor::DrawAxes()
{
	//drawing axes
	glColor3f(_axisColor.R, _axisColor.G, _axisColor.B);
	glVertex2f(_minCoord.X, 0.0f);
	glVertex2f(_maxCoord.X, 0.0f);
	glVertex2f(0.0f, _minCoord.Y);
	glVertex2f(0.0f, _maxCoord.Y);
}