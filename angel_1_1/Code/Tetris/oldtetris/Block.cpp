#include "Block.h"
#include "Logger.h"

static void AdjustS( Square& square, int i=1 )
{
   while(i)
   {
      square.m_bob.y += SQUARE_WIDTH;
      --i;
   }
}
static void AdjustN( Square& square, int i=1 )
{
   while(i)
   {
      square.m_bob.y -= SQUARE_WIDTH;
      --i;
   }
}
static void AdjustW( Square& square, int i=1 )
{
   while(i)
   {
      square.m_bob.x -= SQUARE_WIDTH;
      --i;
   }
}
static void AdjustE( Square& square, int i=1 )
{
   while(i)
   {
      square.m_bob.x += SQUARE_WIDTH;
      --i;
   }
}
static void AdjustSW( Square& square )
{
   AdjustS(square);
   AdjustW(square);
}
static void AdjustSE( Square& square )
{
   AdjustS(square);
   AdjustE(square);
}
static void AdjustNW( Square& square )
{
   AdjustN(square);
   AdjustW(square);
}
static void AdjustNE( Square& square )
{
   AdjustN(square);
   AdjustE(square);
}
static int Greater( int a, int b )
{
   return (a > b ? a : b);
}
static int Lesser( int a, int b )
{
   return (a < b ? a : b);
}



Block::Block(const Logger& logger) :
   m_logger(logger)
{
   m_logger.logTrace( "Block()" );
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		m_squares[i].m_rotIndex = EMPTY;
}

Block::Block( const Block& block) :
   m_logger(block.m_logger)
{
	m_type = block.m_type;
	m_x = block.m_x;
	m_y = block.m_y;
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{	
		m_squares[i].m_rotIndex = block.m_squares[i].m_rotIndex;
		m_squares[i].m_col = block.m_squares[i].m_col;
		m_squares[i].m_row = block.m_squares[i].m_row;

      // HAUKAP - something needs to be done about this....
      Block* non_const_block = const_cast<Block*>(&block);
		Clone_BOB( &(non_const_block->m_squares[i].m_bob), &(m_squares[i].m_bob) );
	}
}
Block::~Block()
{
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		Destroy_BOB(&(m_squares[i].m_bob));
}

// sets the current animation
void Block::SetAnimation(int index)
{
	for( int i=0; i< MAX_SQUARES_PER_BLOCK; ++i )
		Set_Animation_BOB( &(m_squares[i].m_bob), index);
}

// animates the frames of each square
void Block::Animate()
{	
	// animate
	for( int i=0; i< MAX_SQUARES_PER_BLOCK; ++i )
		Animate_BOB( &(m_squares[i].m_bob) );
}

// draws the squares of each block
void Block::Draw()
{
	// draw
	for( int i=0; i< MAX_SQUARES_PER_BLOCK; ++i )
		if( m_squares[i].m_rotIndex != EMPTY )
			Draw_BOB16(&(m_squares[i].m_bob), lpddsback);
}

// creates the block of type 'type'
void Block::FillToType( BlockType type )
{
	switch( type )
	{
	case LineBlock:	// line block
      {
         m_squares[1].m_rotIndex = NORTH;
			AdjustN( m_squares[1] );
			m_squares[2].m_rotIndex = SOUTH;
			AdjustS( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTH1;
			AdjustS( m_squares[3],2 );
      }
      break;
	case ElbowBlock:	
      {
         m_squares[1].m_rotIndex = WEST;
			AdjustW( m_squares[1] );
			m_squares[2].m_rotIndex = EAST;
			AdjustE( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTH;
			AdjustS( m_squares[3] );
      }
      break;
	case CornerLeft:	
      {
         m_squares[1].m_rotIndex = WEST;
			AdjustW( m_squares[1] );
			m_squares[2].m_rotIndex = EAST;
			AdjustE( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTHWEST;
			AdjustSW( m_squares[3] );
      }
      break;
	case CornerRight:	
      {
         m_squares[1].m_rotIndex = WEST;
			AdjustW( m_squares[1] );
			m_squares[2].m_rotIndex = EAST;
			AdjustE( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTHEAST;
			AdjustSE( m_squares[3] );
      }
      break;
	case SquareBlock:	
      {
         m_squares[1].m_rotIndex = EAST;
			AdjustE( m_squares[1] );
			m_squares[2].m_rotIndex = SOUTH;
			AdjustS( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTHEAST;
			AdjustSE( m_squares[3] );
      }
      break;
	case SlantLeft:	
      {
         m_squares[1].m_rotIndex = WEST;
			AdjustW( m_squares[1] );
			m_squares[2].m_rotIndex = SOUTH;
			AdjustS( m_squares[2] );
			m_squares[3].m_rotIndex = SOUTHEAST;
			AdjustSE( m_squares[3] );
      }
      break;
	case SlantRight:
      {
         m_squares[1].m_rotIndex = EAST;
         AdjustE( m_squares[1] );
         m_squares[2].m_rotIndex = SOUTH;
         AdjustS( m_squares[2] );
         m_squares[3].m_rotIndex = SOUTHWEST;
         AdjustSW( m_squares[3] );
      }
      break;
	}
}

void Block::TrackRowColumn()
{
	int row,col;
	for(int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		row = (int)m_squares[i].m_bob.x - GAME_ORIGIN_X;
		col = (int)m_squares[i].m_bob.y - GAME_ORIGIN_Y;
		row /= SQUARE_WIDTH;
		col /= SQUARE_WIDTH;
		m_squares[i].m_row = row;
		m_squares[i].m_col = col;
	}
}

// restructures the block to the new type of block.
void Block::ReloadBlock( BlockType type, BOB_PTR block, int x, int y )
{
	m_type = type;
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		Clone_BOB( block, &(m_squares[i].m_bob));

	//edit location of main block
	m_squares[0].m_rotIndex = CENTER;
	for(int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		m_squares[i].m_bob.x = (float)x ;
		m_squares[i].m_bob.y = (float)y ;
		// compute the index into the emptyarray		
	}
	m_x = x;
	m_y = y;

	// organize the remaining m_squares to their proper locations
	// according to the passed in 'type'
	FillToType( m_type );

	//keep track of their row and column
	TrackRowColumn();

}

// creates the block and all associated BOBs
void Block::CreateBlock(BlockType type, BOB_PTR block, int x, int y)
{
	m_type = type;
	// load square bitmap
		
	// clone the squares

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
		Clone_BOB( block, &(m_squares[i].m_bob));


    //edit location of main block
	m_x = x;
	m_y = y;
	m_squares[0].m_rotIndex = CENTER;
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		m_squares[i].m_bob.x = (float)x;
		m_squares[i].m_bob.y = (float)y;
	}


	// organize the remaining squares to their proper locations
	// according to the passed in 'type'

	FillToType( m_type );
    
	//keep track of their row and column
	TrackRowColumn();
}

// returns the ceiling of the general type of square.
// that is: Given the adjacent squares, N =0, E=1, S=2, W=3
//	if 'square' is one of those four types, this function returns
//  the highest value (W).
int Block::GetGeneralSquareType(int square)
{
	if( square <= TYPE_ADJ )
		return TYPE_ADJ;
	else if( square <= TYPE_CORNER )
		return TYPE_CORNER;
	else
		return TYPE_ADJ1;
}

// rotates the block CW or CCW.

void Block::RotateBlock( RotDirection dir )
{
	// no need to rotate square blocks
	if( m_type == SquareBlock )
		return;

	int general_type;
	

	// for each square in the block, first adjust the x,y coords
	// then change the location relative to the Center block
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		// if the block isn't the center, then rotate
		if( m_squares[i].m_rotIndex != CENTER )
		{
			// adjust the x,y coords first
			m_squares[i].m_bob.x += ROTATION_INDEX[m_squares[i].m_rotIndex][dir+X_VAL];
			m_squares[i].m_bob.y += ROTATION_INDEX[m_squares[i].m_rotIndex][dir+Y_VAL];
			
			TrackRowColumn();
		
			// now adjust the relative location
			general_type = GetGeneralSquareType(m_squares[i].m_rotIndex);
         if( dir == CW )
			{	
            if( ++m_squares[i].m_rotIndex > general_type ) 
					m_squares[i].m_rotIndex = general_type-3;
         }
			else
			{	
            if( --m_squares[i].m_rotIndex < general_type-3 ) 
					m_squares[i].m_rotIndex = general_type;
         }
		}
	}
}

// if passed a direction for X (up or down), moves block direction * xv
// same for y
void Block::MoveBlock( int x_dir, int y_dir, int xv, int yv )
{
	if( x_dir )
	{	
		m_x += x_dir*xv;
		for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
			m_squares[i].m_bob.x += x_dir*xv;
	}
	if( y_dir )
	{	
		m_y += y_dir*yv;
		for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
			m_squares[i].m_bob.y += y_dir*yv;
	}
	
	TrackRowColumn();	
}

// returns true if the block has been split as a result
// of 1 or more rows being deleted from the block
bool Block::IsSplitNeeded(int col)
{ 
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
   {
      if( m_squares[i].m_col > col && m_squares[i].m_rotIndex != EMPTY )
			return true;
   }
	return false;
}

// returns true if each square in the block has been set to empty
// false otherwise
bool Block::IsEmpty()
{
   for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
   {
      if( m_squares[i].m_rotIndex != EMPTY )
			return false;
   }
	return true;
}

// splits the block into two along the row 'y'
// new_block is the receiving ptr for the newly created block
Block* Block::DoSplit(int col)
{
	Block* new_block = new Block(*this);

	// delete all rows below 'col' in this block
	// remember, rows of larger value are lower.
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
   {
      if( m_squares[i].m_col > col )
			m_squares[i].m_rotIndex = EMPTY;
   }

	// delete all rows above 'col' in the new block
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
   {
      if( new_block->GetCol(i) < col )
			new_block->SetSquareType(i, EMPTY);
   }

	return new_block;
}

// sets the m_rotIndex of each square to -1 if their 'y' is equal to 'y'
void Block::EmptyThisRow(int y)
{
    for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
    {
       if( m_squares[i].m_col == y )
			m_squares[i].m_rotIndex = EMPTY;
    }
}
// retrieves how far (in pixels) beyond the desired wall.  accounts for only
// the square farthest from the wall
int Block::GetWallProximity(int whichWall)
{
	int tmp = 0;
	int tmp2 = 1000;
	int final = -1;
	int tmpx, tmpy;

	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{
		// if the square is empty... skip it
		if( m_squares[i].m_rotIndex == -1 )
			continue;

		// keep track of the square furthest from the wall... if and only if
		// it is beyond the wall
		tmpx = (int)m_squares[i].m_bob.x;
		tmpy = (int)m_squares[i].m_bob.y;
		switch(whichWall)
		{
		case WALL_WEST:	
         {
            if( tmp2 > tmpx && tmpx < WALLS::west )
               final = tmp2 = tmpx;
         }
			break;
		case WALL_NORTH:
         {
            if( tmp2 > tmpy && tmpy < WALLS::north )
               final = tmp2 = tmpy;
         }
			break;
		case WALL_EAST:
         {
            if( tmp < (SQUARE_WIDTH + tmpx) && (SQUARE_WIDTH + tmpx) > WALLS::east )
               final = tmp = (SQUARE_WIDTH + tmpx);
         }
         break;
		case WALL_SOUTH:
         {
            if( tmp < (SQUARE_WIDTH + tmpy) && (SQUARE_WIDTH + tmpy) > WALLS::south )
               final = tmp = (SQUARE_WIDTH + tmpy);
         }
			break;
		default:
         assert(!"Invalid Wall");
         break;
		}
	}

	// if finall = -1, there isn't any square which exists beyond the 
	// specified wall, so return -1 (error basically)
	if( final == -1) 
		return final;

	//there is a square beyond the specified wall, so return the difference
	switch(whichWall)
	{
	case WALL_WEST: 
		return WALLS::west - final;
	case WALL_NORTH: 
		return WALLS::north - final;
	case WALL_EAST: 
		return final - WALLS::east;
	case WALL_SOUTH: 
		return final - WALLS::south;
	default:
		return final;
	}
}

void Block::ColumnCorrection()
{
	for( int i=0; i<MAX_SQUARES_PER_BLOCK; ++i )
	{	
		// make an adjustment on the y coord of the BOB
		m_squares[i].m_bob.y = (float)(m_squares[i].m_col * SQUARE_WIDTH)+ GAME_ORIGIN_Y ;
	}
}