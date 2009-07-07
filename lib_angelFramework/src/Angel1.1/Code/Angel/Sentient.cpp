#include "stdafx.h"
#include "Sentient.h"
#include "Console.h"
#include "Box2D.h"
#include "TextRendering.h"
#include "DrawUtil.h"

Sentient::Sentient()
{
	_brain.SetActor(this);
}


void Sentient::Update(float dt)
{
	_brain.Update( dt );
	PhysicsEventActor::Update(dt);
}

void Sentient::Render()
{
	PhysicsEventActor::Render();
	_pathFinder.Render();
	_brain.Render();
}

void SentientFactoryDelegate::InitializeDelegate()
{
	PhysicsEventActorFactoryDelegate::InitializeDelegate();
	_shapeType = PhysicsActor::SHAPETYPE_CIRCLE;
}

