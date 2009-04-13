#include "tree/limb.h"
#include "tree/limbNode.h"
#include "tree/branchNode.h"
#include "moveableObject.h"
#include "vector3D.h"
#include "assert.h"

Limb::Limb(const Vector3& position, const Vector3& orientation):
   m_nodes(),
   m_pursuitObject(NULL),
   m_maxLimbNodes(MAX_LIMB_NODES),
   m_state(LimbFree),
   m_curThrashAngle(0.0f)
{
   m_nodes.push_back( new LimbNode(position,orientation,NULL) );
}
Limb::~Limb()
{
   CHUint sz = (CHUint)m_nodes.size();
   for( CHUint i=0; i<sz; ++i )
   {
      delete m_nodes[i];
   }
   m_nodes.clear();
}

void Limb::growBranches()
{
   CHUint sz = (CHUint)m_nodes.size();
   for(CHUint i=0; i<sz; ++i)
      m_nodes[i]->growBranches();
}
void Limb::removeTarget()
{
   assert( m_state != LimbThrashing );
   if(m_state != LimbThrashing)
   {
      m_pursuitObject= NULL;
      m_state = LimbFree;
   }
}

Vector3 Limb::propogateThrash()
{  
   m_curThrashAngle += MAX_LIMBNODE_ANGLE_PER_FRAME;
   float curAngleIncrease = MAX_LIMBNODE_ANGLE_PER_FRAME;
   LimbNode* node = m_nodes.front();
   Vector3 limbTipStartPosition = m_nodes.back()->position();

   while( node != NULL )
   {      
      Vector3 vec(0.0f, 1.0f, 0.0f);
      Vector3 rotVec = node->orientation().vectorProduct(vec);
      rotVec.invert();
      utils3D::rotateVecByQuat( node->orientation(), rotVec, curAngleIncrease);
      curAngleIncrease *= 1.04f;

      // adjust positions
      Vector3 endPosition = 
         utils3D::translatePointAlongVector( node->position(), 
                                             node->orientation(), 
                                             node->length() );
      node = node->next();
      if(node == NULL)
         break;

      node->setPosition(endPosition);
   }

   return utils3D::getUnitVectorP1ToP2(
      limbTipStartPosition,
      m_nodes.back()->position());
}

void Limb::thrash()
{
   if(m_state != LimbThrashing)
   {
      m_curThrashAngle = 0.0f;
      m_state = LimbThrashing;
      m_pursuitObject->setThrashing();
   }
   
   Vector3 vec = propogateThrash();   

   if(m_curThrashAngle >= PIOver3)
   {
      m_state = LimbFree;
      m_pursuitObject->setFinishedThrash( 
         vec * APPLIED_VELOCITY_MULT );
      m_pursuitObject = NULL;
   }
   else
   {
      m_pursuitObject->setPosition(m_nodes.back()->endPoint());
      // HAUKAP - need rotation here.
   }  
}

bool Limb::targetAquired() const
{
   assert( m_state == LimbPursuing );
   assert( m_pursuitObject != NULL );

   if(m_pursuitObject != NULL)
   {   
      const LimbNode* limbLeaf = m_nodes.back();
      Vector3 endPoint = limbLeaf->endPoint();

      // HAUKAP - bounding spheres?
      BoundingCube boundingCube = m_pursuitObject->getBoundingCube();
      if(endPoint.x <= boundingCube.uX &&
         endPoint.x >= boundingCube.lX &&
         endPoint.y <= boundingCube.uY &&
         endPoint.y >= boundingCube.lY &&
         endPoint.z <= boundingCube.uZ &&
         endPoint.z >= boundingCube.lZ)
      {
         return true;
      }
   }
   return false;
}
float Limb::totalLimbLength() const
{
   float len = 0.0f;
   LIMBNODES::const_iterator curLimbNode = m_nodes.begin();
   while( curLimbNode != m_nodes.end() )
   {
      len += (*curLimbNode)->length();
      ++curLimbNode;
   }   
   return len;
}

LimbNode*
Limb::findFurthestParentNeededToRotate(const MoveableObject& pursuitObject) const
{
   LimbNode* root = m_nodes.front();
   assert(root != NULL);

   float totalLength = 0.0f;
   float distTree2Peep = utils3D::distanceP1P2( root->position(), pursuitObject.position() );
   
   LimbNode* curNode = m_nodes.back();
   while( curNode != root && 
          totalLength < distTree2Peep )
   {
      totalLength += curNode->length();
      curNode = curNode->prev();
   }

   float distParent2Peep = utils3D::distanceP1P2( curNode->position(), pursuitObject.position() );
   while( distParent2Peep > distTree2Peep &&
          curNode != root )
   {
      distParent2Peep = utils3D::distanceP1P2( curNode->position(), pursuitObject.position() );
      curNode = curNode->prev();
   }

   return curNode;
}

void Limb::move()
{   
   assert(m_pursuitObject != NULL);

   LimbNode* node = findFurthestParentNeededToRotate(*m_pursuitObject);
   while( node != NULL )
   {      
      Vector3 vec = utils3D::getUnitVectorP1ToP2(node->position(), 
                                                 m_pursuitObject->position());
      Vector3 rotVec = node->orientation().vectorProduct(vec);
      rotVec.invert();
      utils3D::rotateVecByQuat( node->orientation(), rotVec, MAX_LIMBNODE_ANGLE_PER_FRAME);

      // adjust positions
      Vector3 endPosition = 
         utils3D::translatePointAlongVector( node->position(), 
                                             node->orientation(), 
                                             node->length() );
      node = node->next();
      if(node == NULL)
         break;

      node->setPosition(endPosition);
   }
}

void Limb::assignTarget( MoveableObject& target )
{
   m_pursuitObject = &target;
   m_pursuitObject->setTargeted();
   m_state = LimbPursuing;
}

// returns the leaf node of the limb (a new node if a new one was created, or itself)
void Limb::grow()
{
   assert( m_pursuitObject != NULL );
   assert( !m_nodes.empty() );
   {
      LimbNode* curNode = m_nodes.back();
      assert(curNode != NULL);

      curNode->grow();
      if( curNode->maxLength() && (CHUint)m_nodes.size() < m_maxLimbNodes )
      {
         Vector3 endPoint = curNode->endPoint();
         // generate a new limbnode
         Vector3 orientation = 
            utils3D::getRandomizedUnitVectorP1ToP2(endPoint, 
                                                   m_pursuitObject->position(), 
                                                   (float)LIMB_GROWTH_ANGLE_FUDGE);
         orientation.normalise();

         LimbNode* newNode = new LimbNode(endPoint, orientation, curNode);
         curNode->setNext(newNode);

         // generate side growth for the new limb node
         {          
            int chanceNewGrowth = (rand()%10) +1; // 1-10
            if(chanceNewGrowth <= CHANCE_NEW_GROWTH)         
            {
               int numNewGrowth = rand()% MAX_NEW_GROWTH + 1;
               std::vector<Vector3> branchVectors = 
                  BranchUtils::genBranchesVectors(orientation, numNewGrowth);

               assert( (CHUint)numNewGrowth == (CHUint)branchVectors.size() );
               for( int i=0; i<numNewGrowth; ++i )
                  newNode->addBranch( new BranchNode(curNode->position(), branchVectors[i]));
            }      
         }

         // record the new limbnode
         m_nodes.push_back(newNode);
         increaseWidth();
      }
   }
}

void Limb::increaseWidth()
{
   LIMBNODES::iterator iter = m_nodes.begin();
   while( iter != m_nodes.end() )
   {
      (*iter)->increaseWidth();
      ++iter;
   }
}

void Limb::render() const
{
   CHUint sz = (CHUint)m_nodes.size();
   for( CHUint i=0; i<sz; ++i )
      m_nodes[i]->render();
}