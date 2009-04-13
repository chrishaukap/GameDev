#pragma once

#include "Block.h"
#include "ball.h"

#include <list>
using namespace std;

namespace Archnoid
{


class BlockCollection
{
   typedef std::list<Block*>  BLOCK_LIST;
public:
   BlockCollection(void);
   ~BlockCollection(void);
   
   const Block* GetFirstBlock() const;
   const Block* GetNextBlock() const;

   CollisionType IsCollision(const Ball& ball);
   USHORT GetNumBlocks() const { return (USHORT)m_blocks.size();}

   // movement control
   void RandomMove(BLOCK_LIST::const_iterator iter);
   void RandomMoveAll();
   void RandomGravitateToCenter(BLOCK_LIST::const_iterator iter);
   void RandomGravitateToCenterAll();
   void RandomGravitateToPoint(BLOCK_LIST::const_iterator iter, USHORT x, USHORT y);
   void RandomGravitateToPointAll(USHORT x, USHORT y);

   // birth
   void NewGenerations();
private:
   BLOCK_LIST                 m_blocks;
   mutable BLOCK_LIST::const_iterator m_blockIter;
};

} // end namespace