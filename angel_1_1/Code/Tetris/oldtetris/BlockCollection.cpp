#include "BlockCollection.h"
#include "Logger.h"

#include "t3dlib1.h"

// default constructor
BlockCollection::BlockCollection(const Logger& logger) :
   m_logger(logger)
{
   m_logger.logTrace( "BlockCollection::BlockCollection()" );
	CreateCollection();
}
BlockCollection::~BlockCollection()
{
   m_logger.logTrace( "BlockCollection::~BlockCollection()" );
	// remove all blocks from the list (the collection)
	DeleteCollection();
}
void BlockCollection::CreateCollection()
{
   m_logger.logDebug( "BlockCollection::CreateCollection" );

	for( int i=0; i<8; i++ )
   {	
      m_completionArray[i] = EMPTY;
   }
	
	
	for( int i=0; i<NUM_X_POSITIONS; ++i )
   {
      for( int j=0; j<NUM_Y_POSITIONS; ++j )
		{	
			m_collisionArray[i][j].empty = EMPTY;
			m_collisionArray[i][j].block = NULL;
		}
   }
}
void BlockCollection::DeleteCollection()
{
   m_logger.logTrace( "BlockCollection::DeleteCollection()" );

   list<BLOCK_PTR>::iterator iter = m_collection.begin();
   while( iter != m_collection.end() )
   {
      delete (*iter);
      m_collection.pop_front();
      iter = m_collection.begin();
   }
}
// tests to see if there is a completed row that exists.
bool BlockCollection::IsRowComplete( int y )
{
	for( int x=0; x<NUM_X_POSITIONS; ++x )
		if( m_collisionArray[x][y].empty == EMPTY )
			return false;
	return true;
}

void BlockCollection::SetAnimation(int index)
{
   m_logger.logTrace( "BlockCollection::SetAnimation(int index)" );

	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
   {

		 for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
       {
          for( int j=0; j<8; ++j )
          {
             if( (*m_collectionIter)->GetSquareType(i) != EMPTY &&
					(*m_collectionIter)->GetCol(i) == m_completionArray[j])
             {
                Set_Animation_BOB((*m_collectionIter)->GetBOB(i),index);
             }
          }
       }
   }		 
}

// draws all the BOBS to the screen
void BlockCollection::Animate()
{
	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
   {
      (*m_collectionIter)->Animate();
   }
}


void BlockCollection::Draw()
{
	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
	{
		(*m_collectionIter)->Draw();
	}
}


// Adds the Block (which has collided with the Collection)
// to the Collection at the required coordinates
// returns the point value of the block added
// HAUKAP - are we leaking here?  we create a new block... do we delete the incoming one outside this function>?
int BlockCollection::AddBlock( BLOCK_PTR block )
{   
   m_logger.logTrace( "BlockCollection::AddBlock( BLOCK_PTR block  )" );

	int row,col;
	bool completeRow = false;
	
	// create a new block... to add to the collection list
	BLOCK_PTR newBlock = new Block(*block);
	m_collection.push_back( newBlock );

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{	
		row = block->GetRow(i);
		col = block->GetCol(i);
		m_collisionArray[row][col].empty = FULL;
		// provide a pointer to the block that exists in this square
		m_collisionArray[row][col].block = newBlock;
	}
	//newBlock->SetCntrX(newBlock->GetSquareX(0));
	//newBlock->SetCntrY(newBlock->GetSquareY(0));
	newBlock->SetX(newBlock->GetSquareX(0));
	newBlock->SetY(newBlock->GetSquareY(0));

	PrepRowDelete();
	return POINT_VALUES[block->GetBlockType()];
}

// adds all completed rows to the list of rows to be deleted
// sets a flag to true (a rows is complete)
int BlockCollection::PrepRowDelete()
{
   m_logger.logTrace( "BlockCollection::PrepRowDelete()" );
	int num_complete_rows =0;
	for( int y=0; y<NUM_Y_POSITIONS;  ++y )
	{			
		if( IsRowComplete(y) )
		{		
			++num_complete_rows;
			AddRowToDelete( y );
			m_rowCompleted = true;
		}
	}
	return num_complete_rows;
}

// recursively cascades all blocks after 1 or more row has been 
// deleted.  first deletes rows.... then, all blocks fall as far as
// possible, then recur if another completed row was created.
void BlockCollection::Cascade()
{
   m_logger.logTrace( "BlockCollection::Cascade()" );
	int row;
	BLOCK_PTR tmp = NULL;
	bool collision = false;


	// STEP 1: DELETE ALL ROWS IN COLLISION_ARRAY
	while( (row = GetLowestRow()) != EMPTY )
	{
		for( int x=0; x<NUM_X_POSITIONS;  )
		{
			tmp = m_collisionArray[x][row].block;
			DoDeletion( tmp, x, row );	
			m_collisionArray[x][row].block = NULL;
			m_collisionArray[x][row].empty = EMPTY;

			// advance to the next block in the row (make sure you NULL
			// out each square of the current block as you go
			while( x < NUM_X_POSITIONS && tmp == m_collisionArray[++x][row].block )
			{
				m_collisionArray[x][row].block = NULL;
				m_collisionArray[x][row].empty = EMPTY;
			}
		}
	}
	m_rowCompleted = false;
	ResetCompletionArray();


	// STEP 2: ALL BLOCKS FALL IF POSSIBLE
	MakeBlocksFall();
/*
	BLOCK_PTR block = 0;
	
	for( m_collectionIter = TheCollection.begin();
		 m_collectionIter != TheCollection.end();
		 ++m_collectionIter )
	{
		collision = false;

		while( !collision )
		{
			AdjustCollisionArray( (*m_collectionIter), SUBTRACT_BOOL);
			
			//if( Collisions( (*m_collectionIter), SOUTH ) )
			if( CollisionDown( (*m_collectionIter) ) )
				collision = true;
			else
				(*m_collectionIter)->MoveBlock(NO_CHANGE,DOWN,NO_CHANGE,SQUARE_WIDTH);	
			
			AdjustCollisionArray( (*m_collectionIter), ADD_BOOL);
		}
	}	
*/
}

void BlockCollection::MakeBlocksFall()
{
   m_logger.logTrace( "BlockCollection::MakeBlocksFall()" );

	bool blockFell = false;
	bool collision = false;

	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
	{
		collision = false;

		while( !collision )
		{
			AdjustCollisionArray( (*m_collectionIter), SUBTRACT_BOOL);
			
			//if( Collisions( (*m_collectionIter), SOUTH ) )
			if( CollisionDown( (*m_collectionIter) ) )
				collision = true;
			else
			{
				(*m_collectionIter)->MoveBlock(NO_CHANGE,DOWN,NO_CHANGE,SQUARE_WIDTH);	
				blockFell = true;
			}
			
			AdjustCollisionArray( (*m_collectionIter), ADD_BOOL);
		}
	}	

	if( blockFell )
		MakeBlocksFall();
}

// runs the deletion of the rows whenever a row is completed
// takes cares of the possibility of a split block
void BlockCollection::DoDeletion( BLOCK_PTR block, int x, int y )
{
   m_logger.logTrace( "BlockCollection::DoDeletion( BLOCK_PTR block, int x, int y )" );

	// this will point to the southern end of the split block
	// if a split becomes necessary
	BLOCK_PTR new_block = NULL; 

	// empty out the completed row
	block->EmptyThisRow( y );
	
	// if the block needs to be split in halves, then do so	
	if( block->IsSplitNeeded( y ) )
	{
		new_block = block->DoSplit( y );		
		AddSplitBlock( new_block, block );
	}
	if( block->IsEmpty() )
		RemoveBlock( block );

        
}

// adds the new child block 'new_block' just below its parent 'block'
void BlockCollection::AddSplitBlock(BLOCK_PTR new_block, BLOCK_PTR block)
{
   m_logger.logTrace( "BlockCollection::AddSplitBlock(BLOCK_PTR new_block, BLOCK_PTR block)" );

	int row,col;
	BLOCK_PTR tmp = NULL;
	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
	{
		if( (*m_collectionIter) == block )
		{	
			m_collection.insert( m_collectionIter, new_block );
			break;
		}

	}
	// adjust the collision array for the new block
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = new_block->GetRow(i);
		col = new_block->GetCol(i);
		if( new_block->GetSquareType(i) != EMPTY )
			m_collisionArray[row][col].block = new_block;
	}
}

// removes the block from The collection
void BlockCollection::RemoveBlock(BLOCK_PTR block)
{
   m_logger.logTrace( "BlockCollection::RemoveBlock(BLOCK_PTR block)" );
	
	for( m_collectionIter = m_collection.begin();
		 m_collectionIter != m_collection.end();
		 ++m_collectionIter )
	{
		if( (*m_collectionIter) == block )
		{	
			delete block;
			m_collection.erase( m_collectionIter );
			break;
		}
	}	
}

// adds the row (which needs to be deleted) to
// the completion_array (assuring no duplicates).
// later this array will be used to delete all the rows
// the user has completed.
void BlockCollection::AddRowToDelete(int row)
{
   m_logger.logTrace( "BlockCollection::AddRowToDelete(int row)" );

	// for each element in the completion array
	for (int i=0; i<MAX_SQUARES_PER_BLOCK; ++i)
	{
		// if the element already exists in the array, 
		// simply return, there is no need for duplicates
		if( m_completionArray[i] == row )
			return;
		// add the row to the next available area
		if( m_completionArray[i] == EMPTY )
		{
         m_completionArray[i] = row;
         break;
      }
	}
}

// empties out the completion array (which temporarily tracks
// the specific rows (in the block) that needs to be deleted
// as a result of a block falling into its final position)
void BlockCollection::ResetCompletionArray()
{
   m_logger.logTrace( "BlockCollection::ResetCompletionArray()" );

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		m_completionArray[i] = EMPTY;
}

// tests the emptiness of the CompletionArray.
// returns false if it is NOT empty
// true otherwise
bool BlockCollection::AllSquaresDeleted()
{
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		if( m_completionArray[i] != EMPTY )
			return false;
	return true;
}

// returns the lowest row index (the highest int)
int BlockCollection::GetLowestRow()
{
   m_logger.logTrace( "BlockCollection::GetLowestRow()" );

	int tmp = -1,index=0;
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{	
		if( tmp < m_completionArray[i] ) 
		{
			tmp = m_completionArray[i];
			index =i;
		}
	}
   m_completionArray[index] = EMPTY;
	return tmp;
}

// whenever a block in TheCollection moves (usually from cascading)
// we have to adjust their respective locations in the collision array
// ASSUMPTION: the amount the square is moved, is equal to SQUARE_WIDTH
void BlockCollection::AdjustCollisionArray( BLOCK_PTR block, bool addOrSub)
{	
   m_logger.logTrace( "BlockCollection::AdjustCollisionArray( BLOCK_PTR block, bool addOrSub)" );

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
   {
      if( !block->IsSquareEmpty(i) )
		{
			if( addOrSub ) // add the block to the collision array			
			{
				m_collisionArray[block->GetRow(i)][block->GetCol(i)].block = block;
				m_collisionArray[block->GetRow(i)][block->GetCol(i)].empty = FULL;
			}
			else // subtract the block from the collision array
			{
				m_collisionArray[block->GetRow(i)][block->GetCol(i)].block = NULL;
				m_collisionArray[block->GetRow(i)][block->GetCol(i)].empty = EMPTY;				
			}
		}
   }
}

// checks to see if there has been a collision between any part of block and
// the western wall OR the collection
bool BlockCollection::CollisionLeft( BLOCK_PTR block )
{
   m_logger.logTrace( "BlockCollection::CollisionLeft( BLOCK_PTR block )" );

	int row,col;

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = block->GetRow(i);			
		col = block->GetCol(i);
		
		// if the block is beyond the western wall, there has been a collision
		if( block->GetSquareX(i)< WALLS::west ) 
         return true;

		// if the block at row,col collides with the collection at row,col
		if( m_collisionArray[row][col].empty != EMPTY ) 
         return true;		
	}
	return false; // there were no collisions
}

// checks to see if there has been a collision between any part of block and
// the eastern wall OR the collection
bool BlockCollection::CollisionRight( BLOCK_PTR block )
{
   m_logger.logTrace( "BlockCollection::CollisionRight( BLOCK_PTR block )" );

	int row,col;

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = block->GetRow(i);			
		col = block->GetCol(i);
		
		// if the block is beyond the eastern wall, there has been a collision
		if( block->GetSquareX(i) + SQUARE_WIDTH > WALLS::east ) 
         return true;

		// if the block at row,col collides with the collection at row,col
		if( m_collisionArray[row][col].empty != EMPTY ) 
         return true;		
	}
	return false;// there were no collisions
} 
// checks to see if there has been a collision between any part of block and
// the southern wall OR the collection
bool BlockCollection::CollisionDown( BLOCK_PTR block )
{
   m_logger.logTrace( "BlockCollection::CollisionDown( BLOCK_PTR block )" );
	int16 row,col;


   for( uint16 i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = block->GetRow(i);			
		col = block->GetCol(i);
		
		// if the block is beyond the southern wall, there has been a collision

		
		if( !block->IsSquareEmpty(i) )
		{
			if( block->GetSquareY(i) +SQUARE_WIDTH >= WALLS::south ) 
				return true;

			// if the block at row,col collides with the collection at row,col
			if( col>0 )
				if( m_collisionArray[row][col+1].empty != EMPTY ) 
					return true;	
		}
	}
	return false;// there were no collisions
} 
// checks to see if there has been a collision between any part of block and
// the northern wall OR the collection
bool BlockCollection::CollisionUp( BLOCK_PTR block )
{	
   m_logger.logTrace( "BlockCollection::CollisionUp( BLOCK_PTR block )" );
	int row,col;

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = block->GetRow(i);			
		col = block->GetCol(i);

      //assert( row >= 0 && row < NUM_X_POSITIONS);
      //assert( col >= 0 && col < NUM_Y_POSITIONS);
		
		// if the block is beyond the southern wall, there has been a collision

		
		if( !block->IsSquareEmpty(i) )
		{
			if( block->GetSquareY(i) <= WALLS::north ) 
				return true;

			// if the block at row,col collides with the collection at row,col
			if( col>0 )
				if( m_collisionArray[row][col].empty != EMPTY ) 
					return true;	
		}
	}
	return false;// there were no collisions
}

// determines if there has been a collision between block and
// any part of the collection.
bool BlockCollection::CollisionCollection( BLOCK_PTR block )
{
   m_logger.logTrace( "BlockCollection::CollisionCollection( BLOCK_PTR block )" );
	int row,col;

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = block->GetRow(i);			
		col = block->GetCol(i);
		if( row >=0 && row < NUM_X_POSITIONS )
			if( col >=0 && col < NUM_Y_POSITIONS )
				if( m_collisionArray[row][col].empty != EMPTY ) 
					return true;	
	}
	return false;
}