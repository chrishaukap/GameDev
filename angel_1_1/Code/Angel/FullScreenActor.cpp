#include "StdAfx.h"
#include "FullScreenActor.h"
#include "Camera.h"

FullScreenActor::FullScreenActor()
{
}

void FullScreenActor::Render()
{
	Vector2 maxVert = theCamera.GetWorldMaxVertex();
	Vector2 minVert = theCamera.GetWorldMinVertex();
	Vector2 dimensions = maxVert - minVert;

	SetPosition(Vector2::Zero);
	SetSize(dimensions.X, dimensions.Y);

	return Actor::Render();
}