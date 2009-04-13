#ifndef BRANCHNODE_H
#define BRANCHNODE_H

#include "cdhBase.h"
using namespace CDH;

#include "Vector3D.h"
#include <vector>

class BranchUtils
{
private:
   BranchUtils();
public:
   static Vector3 getPerpindicular(const Vector3& v);
   static std::vector<Vector3> genBranchesVectors(  const Vector3& vec, CHUint numToGen );
};

class BranchNode
{
public:
   typedef std::vector<BranchNode*> CHILDREN;

protected:
   struct SegmentData
   {
   private:
      SegmentData();
      SegmentData(const SegmentData& T);
      const SegmentData& operator=(const SegmentData& T);
   public:
      SegmentData(const Vector3& position, const Vector3& orientation);
      Vector3 m_position;
      float m_length;
      Vector3 m_orientation;   
      float m_width;
      float m_weight;
   };


public:
   BranchNode(const Vector3& position, const Vector3& orientation);
   virtual ~BranchNode();

   Vector3 endPoint() const;

   float length() const;
   void setLength(float len);

   const Vector3& orientation() const;
   void setOrientation(const Vector3& orientation);

   const Vector3& position() const;
   void setPosition(const Vector3& position);
   void refreshPosition(const Vector3& newPosition);

   float width() const;
   void setWidth(float width);

   float growthRate() const;
   void setGrowthRate(float growthRate);

   CHUint getDistFromTreeRoot() const;

   void propogateWidthToRoot();
   void propWidthRec(const BranchNode* callingChild);

   // stale will be true if this branch has stopped growing.
   // in which case, it should be removed from the list of active leaves
   CHILDREN grow(bool& stale);

   // takes ownership of segment
   void addNode(BranchNode* segment);
   void setParent(BranchNode* segment);

   float weight() const;

   void render() const;

protected:
   SegmentData* m_data;
   BranchNode* m_parent; 

private:
   BranchNode();
   BranchNode(const BranchNode& T);
   const BranchNode& operator=(const BranchNode& T);

   CHILDREN m_children;
};

#endif // BRANCHNODE_H