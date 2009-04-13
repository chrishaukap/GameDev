#ifdef _WINDOWS
#include <windows.h>
#endif

#include "quaternion.h"
#include "tree/limbNode.h"
#include "tree/branch.h"
#include <assert.h>

#include <gl/gl.h>
#include <gl/glu.h>

extern GLuint g_texture[];
extern GLUquadricObj *quadratic;

LimbNode::LimbNode(const Vector3& position, 
                   const Vector3& orientation,
                   LimbNode* prev) :
   m_branches(),
   m_position(position),
   m_orientation(orientation),
   m_length(0),
   m_width(MIN_BRANCH_WIDTH),
   m_next(NULL),
   m_prev(prev)
{
}

LimbNode::~LimbNode()
{
   CHUint sz = (CHUint)m_branches.size();
   for( CHUint i=0; i<sz; ++i )
   {
      delete m_branches[i];
   }
   m_branches.clear();
}

void LimbNode::growBranches()
{
   CHUint sz = (CHUint)m_branches.size();
   for( CHUint i=0; i<sz; ++i )
   {
      if(m_branches[i]->canGrow())
         m_branches[i]->grow();
   }
}
void LimbNode::grow()
{
   // HAUKAP - TODO - need to break up the nodes here?
   m_length += BRANCH_GROWTH_RATE;
   if(m_length > MAX_LIMB_LENGTH)
      m_length = MAX_LIMB_LENGTH;
}
void LimbNode::addBranch(BranchNode* branch)
{
   m_branches.push_back(new Branches(branch));
}

void LimbNode::increaseWidth()
{
   if( width() + LIMB_WIDTH_GROWTH <= MAX_LIMB_WIDTH )
      m_width += LIMB_WIDTH_GROWTH;
}

const Vector3& LimbNode::position() const
{
   return m_position;
}

void LimbNode::setPosition(const Vector3& pos)
{
   m_position = pos;
   std::vector<Branches*>::iterator curBranch = m_branches.begin();
   while( curBranch != m_branches.end() )
   {      
      (*curBranch)->refreshPosition( position() );
      ++curBranch;      
   }
}

Vector3 LimbNode::endPoint() const
{
   return utils3D::translatePointAlongVector(position(), orientation(), length());
}
Vector3& LimbNode::orientation()
{
   return m_orientation;
}
const Vector3& LimbNode::orientation() const 
{
   return m_orientation;
}
bool LimbNode::maxLength() const
{
   return (m_length >= MAX_BRANCH_LENGTH) ? true: false;
}

LimbNode* LimbNode::next() const
{
   return m_next;
}
LimbNode* LimbNode::prev() const
{
   return m_prev;
}
void LimbNode::setNext(LimbNode* node)
{
   m_next = node;
}
void LimbNode::setPrev(LimbNode* prev)
{
   m_prev = prev;
}

struct TxtSurface
{
   TxtSurface(const Vector3& llc, const Vector3& lrc, const Vector3& ulc, const Vector3& urc) :
      m_llc(llc), m_lrc(lrc), m_ulc(ulc), m_urc(urc) {}
   const Vector3 &m_llc, &m_lrc, &m_ulc, &m_urc;
};
void LimbNode::render() const
{   
	if(length() > 0)
   {		
      glEnable(GL_TEXTURE_2D);					
	   glDisable(GL_DEPTH_TEST);	 

      glPushMatrix();
      {
         GLuint textureIdx = 6; // HAUKAP - magic numbers.... should get these from a texture manager
         glBindTexture(GL_TEXTURE_2D, g_texture[textureIdx]);	
         GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };	

			Vector3 p1 = position();
			Vector3 ori = orientation();
			Vector3 p2 = endPoint();
			
			Vector3 zAxis(0.0f, 0.0f, 1.0f);
			
			float angle = acos(ori.scalarProduct(zAxis));
			Vector3 axis = zAxis.vectorProduct(ori);	

			float startrad = width() * 0.5f;
			float endrad = m_next ? m_next->width()*0.5f : 0.0f;

			glTranslatef(p1.x, p1.y, p1.z);					
			glRotatef(RAD2DEG(angle), axis.x, axis.y, axis.z);	
			glBindTexture(GL_TEXTURE_2D, g_texture[6]);
			gluCylinder(quadratic, startrad, endrad, length(), 32, 32);
      }
      glPopMatrix();
      
      glEnable(GL_DEPTH_TEST);		
      glDisable(GL_TEXTURE_2D);         
   }

   CHUint sz = (CHUint)m_branches.size();
   for( CHUint i=0; i<sz; ++i )
      m_branches[i]->render();
}
