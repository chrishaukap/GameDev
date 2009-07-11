
#include "Money.h"
#include <assert.h>

// HAUKAP - for cheezy collision
#include "Tim.h"
#include "Indie.h"
#include "World.h"

using namespace CDH;
using namespace Edge;

Money::Money() :
   m_state(eExists)
{
	SetColor(0,1,0);
}
Money::~Money()
{}

void Money::Collided(const Tim*)
{
	m_state = eDeleteMe;
	theWorld.Remove( this );
}
void Money::Collided(const Indie*)
{
	m_state = eDeleteMe;
	theWorld.Remove( this );
}
