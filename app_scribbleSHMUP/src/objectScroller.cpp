#include "objectScroller.h"

#include "cdhBase.h"
#include "actor/spatialObject.h"
#include <assert.h>
#include <vector>

using namespace CDH;

struct ObjectScroller::Data
{
	typedef std::vector<SpatialObject*> ObjectVector;

   Data() : rate(0.0f), objects(), direction(Left) {}
   ~Data(){}

	float rate;
	ObjectVector objects;
	Direction direction;
};

ObjectScroller::ObjectScroller() : m_data(new Data())
{}
ObjectScroller::~ObjectScroller()
{
   delete m_data;
   m_data = NULL;
}

void 
ObjectScroller::setScrollRate(float rate)
{
	assert(m_data != NULL);
	m_data->rate = rate;
}
void 
ObjectScroller::setDirectionToScroll(Direction direction)
{
	assert(m_data != NULL);
	m_data->direction = direction;
}
void
ObjectScroller::clear()
{
   m_data->objects.clear();
}
void 
ObjectScroller::addObject(SpatialObject* objectToScroll)
{
	assert(m_data != NULL);
	m_data->objects.push_back( objectToScroll );
}
void 
ObjectScroller::process()
{
	assert(m_data != NULL);
	CHUint sz = (CHUint)m_data->objects.size();
	for( CHUint i=0; i<sz; ++i )
	{
      Vector3 newPosition = m_data->objects[i]->position();
      switch(m_data->direction)
      {
      case Left: newPosition.x -= m_data->rate; break;
      case Right: newPosition.x += m_data->rate; break;
      default: assert( !"Invalid direction!");
      }

      m_data->objects[i]->setPosition(newPosition);
   }
}