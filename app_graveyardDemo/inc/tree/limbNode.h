#ifndef LIMBNODE_H
#define LIMBNODE_H

#include "vector3d.h"
#include <vector>

// as Limb grows, adds new limbnodes.  at each segment, new branches may spawn
class Branches;
class BranchNode;
class LimbNode
{
public:
   LimbNode(const Vector3& position, 
            const Vector3& orientation,
            LimbNode*);
   ~LimbNode();

   float length() const {return m_length;}
	float width() const {return m_width;}
   void increaseWidth();
   void grow();
   void growBranches();

   const Vector3& position() const;
   void setPosition(const Vector3& position);

   Vector3 endPoint() const;
   Vector3& orientation();
   const Vector3& orientation() const;
   bool maxLength() const;

   // takes ownership of branch
   void addBranch(BranchNode* branch); // branch becomes the root branchnode in the branch
   void render() const;

   LimbNode* next() const;
   LimbNode* prev() const;
   void setNext(LimbNode* node);
   void setPrev(LimbNode* node);

private:
   LimbNode();
   LimbNode(const LimbNode& T);
   const LimbNode& operator=(const LimbNode& T);

   std::vector<Branches*> m_branches;
   float m_length;
   float m_width;
   Vector3 m_position;
   Vector3 m_orientation;
   LimbNode* m_next;
   LimbNode* m_prev;
};

#endif // LIMBNODE_H