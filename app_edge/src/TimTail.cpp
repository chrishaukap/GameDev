
#include "TimTail.h"
#include "TimHead.h"
#include "MathUtil.h"
#include "Actor.h"
#include "World.h"
#include "MathUtil.h"
#include <vector>

using namespace CDH;
using namespace Edge;

class TailNode : public Actor
{
public:
   void moveTo(const Vector2& point, float distance)
   {
      SetPosition(MathUtil::MoveP1TowardP2(GetPosition(), point, distance));
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

TimTail::TimTail(const TimHead& head) : 
	m_head(head),
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
      TailNode* lastNode = m_data->_nodes[(int)m_data->_nodes.size()-1];
      pos = lastNode->GetPosition();
   }
   else
   {
		pos = m_head.GetPosition();
   }
	pos.X += m_head.GetSize().X;
   node->SetPosition(pos);
	node->SetColor(0,0,1);
   theWorld.Add(node);
   m_data->_nodes.push_back(node);
}
void 
TimTail::moveTo(const Vector2& point, float distance)
{
   int numNodes = (int)m_data->_nodes.size();
   float size = m_head.GetSize().X * 0.90f;
	for( int i=numNodes-1; i>=0; --i )
	{
      Vector2 dstPoint = 
         MathUtil::MoveP1TowardP2(
            i==0 ? point : m_data->_nodes[i-1]->GetPosition(), 
            m_data->_nodes[i]->GetPosition(), 
            size);
		m_data->_nodes[i]->moveTo(dstPoint,distance);
	}
}