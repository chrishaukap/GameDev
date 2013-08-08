
#include "Indie.h"
#include "Util/MathUtil.h"
#include "Infrastructure/VecStructs.h"
#include <assert.h>
#include <algorithm>

using namespace Edge;
#define INDIE_LIFESPAN 750

int Indie::NextTeamNumber = -1;
Indie::Indie() : 
   m_state(eLiving),
   m_dyingCounter(0),
	m_adjacentIndies(),
	m_leader(true),
	m_teamNumber(-1)
{
}

Indie::~Indie()
{}

void
Indie::killMe()
{
	if(!isDead())
		m_state = eDying;	
	SetColor( 0.5f, 1.0f, 0.5f );
}
void 
Indie::assignToNextTeam(const Indie& indie)
{
	m_teamNumber = indie.getTeamNumber();
}
static float randomizeValue( float value, float fudge )
{
   // amount of total fudge factor
   double randPercent = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
   float dAngle = (float)randPercent * fudge;
   
   // + or - fudge factor
   int plusOrMinus = 0;
   plusOrMinus = rand()%2;
   if(plusOrMinus == 0)
      return value + dAngle;
   else
      return value - dAngle;
}

void Indie::move(float dt)
{
	const float dPos = 0, variation = 5.0f;	

	float randX = dt * randomizeValue(dPos, variation);
	float randY = dt * randomizeValue(dPos, variation);
	INDIE_LIST visitedIndies;
	moveRec( visitedIndies, randX, randY);
}
void Indie::moveRec(INDIE_LIST& visited, float dX, float dY)
{
	SetPosition( _position.X + dX, _position.Y + dY);
	visited.push_back(this);

	INDIE_LIST::iterator begin = m_adjacentIndies.begin();
	INDIE_LIST::iterator end = m_adjacentIndies.end();
	INDIE_LIST::iterator iter = begin;
	while( iter != end )
	{
		if( std::find(begin, end, *iter) == end )
			(*iter)->moveRec(visited, dX, dY);
		++iter;
	}
}

void Indie::Update(float dt)
{
   switch(m_state)
   {
   case eLiving:
      {
			if(m_leader)
				move(dt);
      }
      break;     
   case eDying:
      if( ++m_dyingCounter > INDIE_LIFESPAN )
         m_state = eDead;
      break;
   case eDead:
      break;
   case eDeleteMe:
      break;
   default:
      assert(!"Invalid Indie State");
   }
}
bool
Indie::onTheSameTeam(const Indie& indie)
{
	assert(indie.isOnTeam() && isOnTeam());
	return indie.getTeamNumber() == getTeamNumber();
}
void
Indie::setTeamColor()
{
	float r = MathUtil::RandomFloat(), g = MathUtil::RandomFloat(), b = MathUtil::RandomFloat();
	setTeamColorRec(r,g,b);
}
void
Indie::setTeamColorRec(float r, float g, float b)
{
	SetColor(r,g,b);
	int numAdjacents = (int)m_adjacentIndies.size();
	for( int i=0; i<numAdjacents; ++i )
	{
		Color color = m_adjacentIndies[i]->GetColor();
		if( !(color.R == r && color.B ==b && color.G == g) )
			m_adjacentIndies[i]->setTeamColorRec(r,g,b);
	}
}
void
Indie::assignAdjacentsToTeam(const Indie& indie)
{
	int numAdjacents = (int)m_adjacentIndies.size();
	for( int i=0; i<numAdjacents; ++i )
		m_adjacentIndies[i]->assignToNextTeam(indie);
	
}
void 
Indie::joinIndie(Indie& indie)
{
	if(indie.isOnTeam())
	{
		if(indie.getTeamNumber() != getTeamNumber())
		{
			assignToNextTeam(indie);
			assignAdjacentsToTeam(indie);
		}
	}
	else
		assignToNextTeam();
	m_adjacentIndies.push_back(&indie);
}