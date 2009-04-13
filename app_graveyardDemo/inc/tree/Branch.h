#ifndef BRANCH_H
#define BRANCH_H

#include "branchNode.h"

class BranchNode;
class Vector3;
class Branches
{
public:
   // takes ownership of node
   Branches(BranchNode* node);
   ~Branches();

   void grow();

   void render() const;

   float weight() const;
   bool canGrow() const {return weight() < m_maxWeight ? true: false;}
   void refreshPosition(const Vector3& newPosition);

private:
   Branches();
   Branches(const Branches& T);
   const Branches& operator=(const Branches& T);

   BranchNode* m_root;
   
   BranchNode::CHILDREN m_activeLeaves;
   float m_maxWeight;
};

#endif // BRANCH_H