
#include "tree/branch.h"
#include "assert.h"

Branches::Branches(BranchNode* root) :
   m_root(root),
   m_activeLeaves(),
   m_maxWeight(BRANCH_MAX_WEIGHT)
{
   m_activeLeaves.push_back(root);   
}
Branches::~Branches()
{}

void Branches::grow()
{
   std::vector<CHUint> leafIndexesToRemove;

   CHUint sz = (CHUint)m_activeLeaves.size();
   for(CHUint i=0; i<sz; ++i)
   {
      bool stale = false;
      BranchNode::CHILDREN newLeaves = m_activeLeaves[i]->grow(stale);
      if(stale)
         leafIndexesToRemove.push_back(i);
      else
      {
         CHUint szNewLeaves = (CHUint)newLeaves.size();
         for(CHUint j=0; j<szNewLeaves; ++j)
            m_activeLeaves.push_back( newLeaves[j] );
      }
   }

   // remove stale branches from the active list
   sz = (CHUint)leafIndexesToRemove.size();
   for(CHUint i=0; i<sz; ++i)
   {
      if(leafIndexesToRemove[i]+1 > (CHUint)m_activeLeaves.size())
         m_activeLeaves.pop_back();
      else
         m_activeLeaves.erase( m_activeLeaves.begin() + leafIndexesToRemove[i],
                               m_activeLeaves.begin() + leafIndexesToRemove[i] + 1 );
   }
}

void Branches::render() const 
{
   assert( m_root != NULL );
#ifdef RENDER_BRANCHES
   m_root->render();
#endif
}

float Branches::weight() const
{
   assert(m_root != NULL);
   return m_root->weight();
}
void Branches::refreshPosition(const Vector3& newPosition)
{
   m_root->refreshPosition(newPosition);
}


Vector3 BranchUtils::getPerpindicular(const Vector3& v)
{
   float x = abs(v.x);
   float y = abs(v.y);
   float z = abs(v.z);
   if(x<y)
   {
      if(x<z)
         return v.vectorProduct(Vector3(1,0,0));
      else
         return v.vectorProduct(Vector3(0,0,1));
   }
   else
   {
      if(y<z)
         return v.vectorProduct(Vector3(0,1,0));
      else
         return v.vectorProduct(Vector3(0,0,1));
   }
}

std::vector<Vector3> BranchUtils::genBranchesVectors(  const Vector3& vec, CHUint numToGen )
{
   // HAUKAP - TODO - randomize ??
   assert( vec.isNormalised() );
   assert(numToGen >= 1 && numToGen <= 4);

   Vector3 v1 = getPerpindicular(vec); // first branch (on cardinal axis)
   Vector3 v1AsAxis(v1);

   Vector3 v2 = v1.vectorProduct(vec);   // 2nd branch
   Vector3 v2AsAxis(v2);

   Vector3 v3 = v1;
   v3.invert();
   Vector3 v3AsAxis(v3);

   Vector3 v4 = v2;
   v4.invert();
   Vector3 v4AsAxis(v4);

   // HAUKAP - TODO - add to defines
   float angle = 2*PIOver5;
   utils3D::rotateVecByQuat(v1, v4AsAxis, angle); 
   utils3D::rotateVecByQuat(v2, v1AsAxis, angle);   
   utils3D::rotateVecByQuat(v3, v2AsAxis, angle);    
   utils3D::rotateVecByQuat(v4, v3AsAxis, angle);     

   // HAUKAP - TODO - make this more efficient...
   std::vector<Vector3> vectors;
   vectors.push_back(v1);
   if(numToGen >= 2)
      vectors.push_back(v2);
   if(numToGen >= 3)
      vectors.push_back(v3);
   if(numToGen == 4)
      vectors.push_back(v4);

   return vectors;
}