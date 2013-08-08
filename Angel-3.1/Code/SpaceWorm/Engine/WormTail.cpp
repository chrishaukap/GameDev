#include "cdhBase.h"
#include "WormTail.h"
#include "WormHead.h"
#include "Infrastructure/World.h"

#include <vector>

static const float HEAD_SIZE_RATIO = 0.3f;
class TailNode : public Actor
{
public:
   TailNode() 
   {
      SetSprite("Resources/Images/WormBody.png");
   }
   void moveTo(const Vector2& point, float distance)
   {     
      Vector2 vecV1ToV2 = point - GetPosition();
      vecV1ToV2.Normalize();     
      SetPosition(Vector2( GetPosition() + (vecV1ToV2 * distance) ));
   }
};

class WormTail::Data
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

WormTail::WormTail(const WormHead& head, int maxLength) : 
	m_head(head),
   m_data(new Data()),
   m_maxLength(maxLength)
{}
WormTail::~WormTail()
{
   delete m_data; m_data = NULL;
}

void 
WormTail::addTailNode()
{
   if(m_data->_nodes.size() >= static_cast<unsigned int>(m_maxLength))
      return;

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
	pos.X += HEAD_SIZE_RATIO;
   node->SetSize(HEAD_SIZE_RATIO);
   node->SetPosition(pos);
	node->SetColor(1,1,1);
   theWorld.Add(node,9);
   m_data->_nodes.push_back(node);
}
void WormTail::AddToWorld()
{
   int numNodes = (int)m_data->_nodes.size();
	for( int i=numNodes-1; i>=0; --i )
      theWorld.Add(m_data->_nodes[i],2);
}
void 
WormTail::moveTo(const Vector2& point, float distance)
{
   int numNodes = (int)m_data->_nodes.size();
	for( int i=numNodes-1; i>=0; --i )
	{
      const Vector2& pToMove = i==0 ? point : m_data->_nodes[i-1]->GetPosition();
      const Vector2& dstPoint = m_data->_nodes[i]->GetPosition();
      float speed = m_head.GetSize().X * HEAD_SIZE_RATIO;

      Vector2 vecV1ToV2 = dstPoint - pToMove;
      vecV1ToV2.Normalize();

		m_data->_nodes[i]->moveTo(Vector2( pToMove + (vecV1ToV2 * speed)), distance);
	}
}
bool WormTail::IntersectsWithRect( const struct Rect<float>& theirRect) const
{
   int numNodes = (int)m_data->_nodes.size();
	for( int i=numNodes-1; i>=0; --i )
	{    
      Vector2 nodepos = m_data->_nodes[i]->GetPosition();
      Vector2 nodesize = m_data->_nodes[i]->GetSize();
      Point<float>ul(nodepos.X - nodesize.X * 0.5f, nodepos.Y - nodesize.Y * 0.5f);
      Point<float>lr(nodepos.X + nodesize.X * 0.5f, nodepos.Y + nodesize.Y * 0.5f);
      Rect<float> ourRect(ul,lr);
      if(RectanglesIntersect(theirRect, ourRect))
         return true;
   }
   return false;
}