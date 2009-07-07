
#include "TimTail.h"
#include "MathUtil.h"
#include "Actor.h"
#include "World.h"
#include <vector>

using namespace CDH;
using namespace Edge;

class TailNode : public Actor
{
public:
   void moveTo(const Vector2& point, float distance)
   {
      MathUtil::MoveP1TowardP2(GetPosition(), point, distance);
   }
};

class TimTail::Data
{
public:
   typedef std::vector<TailNode*> TAILNODES;
   Data() : _nodes() {}
   ~Data() 
   {
      std::vector<TailNode*>::iterator iter = _nodes.begin();
      while( iter != _nodes.end() )
      {
         theWorld.Remove( (*iter) );
         delete (*iter);
         ++iter;
      }
      _nodes.clear();
   }
   TAILNODES _nodes;
};

TimTail::TimTail() : 
   m_data(new Data())
{}
TimTail::~TimTail()
{
   delete m_data; m_data = NULL;
}

void 
TimTail::addTailNode()
{
   TailNode* node = new TailNode;
   Vector2 pos;
   if( ! m_data->_nodes.empty() )
   {
      TailNode* lastNode = m_data->_nodes[(int)m_data->_nodes.size()];
      pos = lastNode->GetPosition();
   }
   else
   {
   }
   node->SetPosition(pos);
   theWorld.Add(node);
   m_data->_nodes.push_back(node);
}
void 
TimTail::moveTo(const Vector2& point, float distance)
{
   Vector2 dstPoint = point;
   Data::TAILNODES::iterator iter = m_data->_nodes.begin();
   while( iter != m_data->_nodes.end() )
   {
      (*iter)->moveTo(dstPoint,distance);
      ++iter;
      if(iter != m_data->_nodes.end())
         dstPoint = (*iter)->GetPosition();
   }
}