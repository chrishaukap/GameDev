#include "StdAfx.h"
#include "BlockCollection.h"

#include "Block.h"
#include "defs.h"
#include "defaults.h"

#include <time.h>


namespace Archnoid
{

BlockCollection::BlockCollection(void) :
   m_blocks()
{
   srand ( (USHORT)time(NULL) );

   USHORT num = BlockCollectionDefaults::NUMBLOCKS;

   USHORT x,y;
   for(USHORT i=0; i<8; ++i)
   {
      for( USHORT j=0; j<20; ++j )
      {
         x = GameAreaDefaults::ULX + (20 * (j+1));
         y = GameAreaDefaults::ULY + (20 * (i+1));
         m_blocks.push_back(new Block(x,y));
      }
   }
}

BlockCollection::~BlockCollection(void)
{
   m_blocks.clear();
   //USHORT num = (USHORT)m_blocks.size();
   //for(USHORT i=0; i<num; ++i)
   //{
   //   delete m_blocks[i];
   //}
}

const Block* BlockCollection::GetFirstBlock() const
{
   if( m_blocks.size() )
   {
      m_blockIter = m_blocks.begin();
      return *m_blockIter;
   }
   else
      return NULL;
}

const Block* BlockCollection::GetNextBlock() const
{
   if( m_blocks.size() )
   {
      if( ++m_blockIter == m_blocks.end() )
         return NULL;
      else
         return *m_blockIter;
   }
   return NULL;
}



CollisionType BlockCollection::IsCollision(const Ball& ball) 
{   
   CollisionType collision = None;
   BLOCK_LIST::iterator iter = m_blocks.begin();

   while( iter != m_blocks.end() )
   {  
      collision = (*iter)->IsCollision(ball);
      if( collision != None)
      {   
         if( 0== (*iter)->DecrementHP())
         {   
            BLOCK_LIST::iterator iter2Next = iter;
            ++iter2Next;
            m_blocks.erase(iter);
            iter = iter2Next;

            //continue; // Ball does not bounce off (destroys all blocks in a straight line)
            break;      // Ball bounces off the first collision
         }
      }
      ++iter;
   }
   return collision;   
}


void BlockCollection::RandomMove(BLOCK_LIST::const_iterator iter)
{
   (*iter)->SetVectorX( (rand()%3) -1 );
   (*iter)->SetVectorY( (rand()%3) -1 );
   (*iter)->Move();
}

void BlockCollection::RandomMoveAll()
{
   BLOCK_LIST::const_iterator iter = m_blocks.begin();
   while( iter != m_blocks.end() )
      RandomMove(iter++);
}

// block will randomly move with a 1:3 probability of moving (B-lining)
// towards the center (per axis)
void BlockCollection::RandomGravitateToCenter(BLOCK_LIST::const_iterator iter)
{   
   USHORT cntrX = GameAreaDefaults::ULX + GameAreaDefaults::WIDTH / 2;
   USHORT cntrY = GameAreaDefaults::ULY + GameAreaDefaults::HEIGHT / 2;
   RandomGravitateToPoint(iter, cntrX, cntrY);
}

void BlockCollection::RandomGravitateToCenterAll()
{
   BLOCK_LIST::const_iterator iter = m_blocks.begin();
   while( iter != m_blocks.end() )
      RandomGravitateToCenter(iter++);
}

// block block will randomly move with a slightly higher chance of 
// gravitating towards the point selected (x,y)
void BlockCollection::RandomGravitateToPoint(BLOCK_LIST::const_iterator iter,
                                             USHORT x, USHORT y)
{
   // HAUKAP - util functions?
#define PROB_1_IN_4 (rand()%4)<1
#define PROB_1_IN_3 (rand()%3)<1
#define PROB_1_IN_2 (rand()%2)<1
#define PROB_1_IN_1 (rand()%1)<1
#define PROB_METHOD PROB_1_IN_3

   // X first   
   USHORT cntrx = (*iter)->GetX() + (*iter)->GetHeight() / 2;

   // 1 in 3 chance of being true
   if(PROB_METHOD)
      cntrx > x ? (*iter)->SetVectorX( -1 ) : (*iter)->SetVectorX( 1 );
   else
      (*iter)->SetVectorX( (rand()%3) -1 );

   // Y second
   USHORT cntry = (*iter)->GetY() + (*iter)->GetHeight() / 2;
   USHORT BoardcntrY = GameAreaDefaults::ULY + GameAreaDefaults::HEIGHT / 2;
   
   // 1 in 3 chance of being true
   if(PROB_METHOD)
      cntry > y ? (*iter)->SetVectorY( -1 ) : (*iter)->SetVectorY( 1 );
   else
      (*iter)->SetVectorY( (rand()%3) -1 );

   (*iter)->Move();
}

void BlockCollection::RandomGravitateToPointAll(USHORT x, USHORT y)
{
   BLOCK_LIST::const_iterator iter = m_blocks.begin();
   while( iter != m_blocks.end() )
      RandomGravitateToPoint(iter++, x, y);
}

void BlockCollection::NewGenerations()
{
   if( (rand()%1) < 1 ) 
      m_blocks.push_back(new Block( GameAreaDefaults::ULX + 20, GameAreaDefaults::ULY + 20 ));
   if( (rand()%1) < 1 ) 
      m_blocks.push_back(new Block( GameAreaDefaults::ULX + (GameAreaDefaults::WIDTH - 20), GameAreaDefaults::ULY + 20 ));
   if( (rand()%1) < 1 ) 
      m_blocks.push_back(new Block( GameAreaDefaults::ULX + (GameAreaDefaults::WIDTH - 20), GameAreaDefaults::ULY + (GameAreaDefaults::HEIGHT - 20) ));
   if( (rand()%1) < 1 ) 
      m_blocks.push_back(new Block( GameAreaDefaults::ULX + 20, GameAreaDefaults::ULY + (GameAreaDefaults::HEIGHT - 20) ));
}

} // end namespace