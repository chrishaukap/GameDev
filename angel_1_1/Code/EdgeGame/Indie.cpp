#include "StdAfx.h"
#include "Indie.h"
#include "MathUtil.h"

using namespace CDH;
using namespace Edge;

Indie::Indie() 
{}
Indie::~Indie()
{}

void Indie::Update(float dt)
{
	const float dPos = 0;	
	const bool favorUp = true;
	SetPosition( _position.X + dt * MathUtil::randomizeValue(dPos, 5.0f),
					 _position.Y + dt * MathUtil::randomizeValue(dPos, 5.0f));
}