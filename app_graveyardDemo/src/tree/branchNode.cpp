#ifdef _WINDOWS
#include <windows.h>
#endif

#include "assert.h"

#include "tree/branchNode.h"
#include <gl/gl.h>

BranchNode::SegmentData::SegmentData(const Vector3& position, 
                                     const Vector3& orientation) : 
   m_position(position),
   m_length(0.0f),
   m_orientation(orientation),
   m_width(MIN_BRANCH_WIDTH),
   m_weight(BRANCHNODE_WEIGHT)
   {}       

BranchNode::BranchNode(const Vector3& position, const Vector3& orientation) :
   m_parent(NULL), 
   m_children()
{
   m_data = new SegmentData(position, orientation);   
} 
BranchNode::~BranchNode()
{
   delete m_data;
   CHUint sz = (CHUint)m_children.size();
   for( CHUint i=0; i<sz; ++i )
      delete m_children[i];
   m_children.clear();
}

BranchNode::CHILDREN 
BranchNode::grow(bool& stale)
{
   stale = false;
   CHILDREN newLeaves;

   assert(m_data != NULL);
   if(m_data->m_length < MAX_BRANCH_LENGTH)
      m_data->m_length += BRANCH_GROWTH_RATE;
   else
   {            
      int chanceNewGrowth = (rand()%10); // 1-10
      if(chanceNewGrowth <= CHANCE_NEW_GROWTH)         
      {
         int numNewGrowth = rand()% MAX_NEW_GROWTH + 1;
         
         Vector3 theEndPoint = endPoint();

         // first new growth is same orientation as parent
         addNode( new BranchNode(theEndPoint, orientation()) );

         --numNewGrowth;
         if(numNewGrowth >0)
         {
            std::vector<Vector3> branchVectors = 
               BranchUtils::genBranchesVectors(orientation(), numNewGrowth);

            assert( (CHUint)numNewGrowth == (CHUint)branchVectors.size() );
            for( int i=0; i<numNewGrowth; ++i )
            {
               BranchNode* newBranch = new BranchNode( theEndPoint, branchVectors[i]);
               addNode(newBranch);
               newLeaves.push_back(newBranch);
            }
         }
      }
      else
         stale = true;
   }

   return newLeaves;
}

float 
BranchNode::length() const 
{
   assert(m_data != NULL);
   return m_data->m_length;
}

const Vector3&
BranchNode::orientation() const 
{
   assert(m_data != NULL);
   return m_data->m_orientation;
}
void 
BranchNode::setOrientation(const Vector3& orientation) 
{
   assert(m_data != NULL);
   m_data->m_orientation = orientation;
}

const Vector3&
BranchNode::position() const 
{
   assert(m_data != NULL);
   return m_data->m_position;
}

float 
BranchNode::width() const 
{
   assert(m_data != NULL);
   return m_data->m_width;
}


void 
BranchNode::setPosition(const Vector3& position)
{
   assert(m_data != NULL);
   m_data->m_position = position;
}

void BranchNode::refreshPosition(const Vector3& position)
{
   assert( m_data != NULL );
   m_data->m_position = position;
   Vector3 newPosition =
      utils3D::translatePointAlongVector( m_data->m_position,
                                          m_data->m_orientation,
                                          m_data->m_length);

   CHILDREN::iterator curChild = m_children.begin();
   while( curChild != m_children.end() )
   {
      (*curChild)->refreshPosition(newPosition);
      ++curChild;
   }
}

void 
BranchNode::setWidth(float width) 
{
   assert(m_data != NULL);
   m_data->m_width = width;
}

Vector3
BranchNode::endPoint() const
{
   return utils3D::translatePointAlongVector(position(), orientation(), length());
}

// takes ownership of segment
void 
BranchNode::addNode(BranchNode* segment)
{
   segment->setParent(this);
   m_children.push_back(segment);
}

void 
BranchNode::setParent(BranchNode* segment)
{
   assert(segment  != NULL);
   m_parent = segment;
}

float
BranchNode::weight() const
{
   assert(m_data != NULL);
   float runningTotal = m_data->m_weight;

   CHUint sz = (CHUint)m_children.size();
   for(CHUint i=0; i<sz; ++i)
      runningTotal += m_children[i]->weight();

   return runningTotal;
}

void BranchNode::render() const
{   
   glPushMatrix();
   {
      GLfloat color[] = { 0.2f, 0.1f, 0.1f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
      const Vector3& p1 = position();
      Vector3 p2 = endPoint();
      glBegin(GL_LINES);
      {
         glVertex3f( p1.x, p1.y, p1.z );
         glVertex3f( p2.x, p2.y, p2.z );
      }
      glEnd();
   }
   glPopMatrix();

   CHUint sz = (CHUint)m_children.size();
   for( CHUint i=0; i<sz; ++i )
      m_children[i]->render();
}