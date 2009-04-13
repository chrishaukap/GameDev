#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "cdhBase.h"
using namespace CDH;
#include "defines.h"
#include <vector>

struct Camera;
class Tree;
class FenceSegment;
class PeepDriver;
class BranchNode;
class LimbNode;
class BloodEngine;
class AbstractPeep;

class GameEngine
{
public:
   GameEngine();
   ~GameEngine();

   void initializePreCredits();
   void initializePostCredits();
   void process();
   void destroy();
   
   void render() const;

   // camera accessor
   Camera* camera() {return m_camera;}
   const Camera* camera() const {return m_camera;}
   
   // fence accessors
   CHUint numFenceSegments() const { return (CHUint)m_fence.size(); }
   const std::vector<FenceSegment*>::const_iterator getFenceSegments() const;

   // DEBUG --
   CHUint numSpouts() const;
   CHUint numDroplets() const;
   CHUint numPeepParts() const;

private:
   // init
   void initTree();
   void populateFence();
   void assignPeepsForPursuit();

   // destruction
   void destroyFence();

   // processing
   void processTree();
   void processFence();
   void processPeeps();
   void processCamera();
   void processBloodEngine();
   
   Camera* m_camera;
   std::vector<FenceSegment*> m_fence;
   Tree* m_tree;
   BloodEngine* m_bloodEngine;
   PeepDriver* m_peepDriver;
};

#endif // GAMESTATE_H

