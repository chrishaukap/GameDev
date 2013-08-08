
#include "Money.h"
#include <assert.h>

using namespace Edge;

Money::Money() :
   m_state(eExists)
{
	SetColor(0,1,0);
}
Money::~Money()
{}
