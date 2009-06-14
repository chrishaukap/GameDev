#ifndef BLOCK_H
#define BLOCK_H


#include "t3dlib1.h"
#include "globals.h"

#define EMPTY -1
#define FULL 1


// the general square types.  in all blocks except
// the line block, each square is either adjacent to
// the center, or corner to the center square.
// in line blocks, they can also be of type
// adj1 (two squares from the center) or
// adj2 (three squares from the center)
#define TYPE_ADJ 3
#define TYPE_CORNER 7
#define TYPE_ADJ1 11

// the standard blocks of tetrix
typedef enum
{
   LineBlock,  //		DDDD
             
   ElbowBlock, //		DDD
               //		 D

   CornerLeft, //		DDD
               //		D

   CornerRight,//		DDD
               //		  D

   SquareBlock,//		DD
               //		DD

   SlantLeft,  //		DD
               //		 DD

   SlantRight, //		 DD
               //		DD   
   NumBlockTypes
} BlockType;

// index values into the ROTATION_INDEX ARRAY
#define X_VAL 0
#define Y_VAL 1

// with of squares in pixels
#define SQUARE_WIDTH	20

// relative directions (around the center block)
typedef enum
{
   // the order matters!  we use these to index into the ROTATION_INDEX
   NORTH, EAST, SOUTH, WEST, 
   NORTHWEST, NORTHEAST, SOUTHEAST, SOUTHWEST,
   NORTH1, EAST1, SOUTH1, WEST1, CENTER
} BlockLocations;

// rotation index.  used to rotate squares about the center square
const int ROTATION_INDEX[12][4] = 
   {	{SQUARE_WIDTH, SQUARE_WIDTH, -SQUARE_WIDTH, SQUARE_WIDTH},
		{-SQUARE_WIDTH, SQUARE_WIDTH, -SQUARE_WIDTH, -SQUARE_WIDTH},
		{-SQUARE_WIDTH, -SQUARE_WIDTH, SQUARE_WIDTH, -SQUARE_WIDTH},
		{SQUARE_WIDTH, -SQUARE_WIDTH, SQUARE_WIDTH, SQUARE_WIDTH},
		{2*SQUARE_WIDTH, 0, 0, 2*SQUARE_WIDTH},
		{0, 2*SQUARE_WIDTH, -2*SQUARE_WIDTH, 0},
		{-2*SQUARE_WIDTH, 0, 0, -2*SQUARE_WIDTH},
		{0, -2*SQUARE_WIDTH, 2*SQUARE_WIDTH, 0},
		{2*SQUARE_WIDTH, 2*SQUARE_WIDTH, -2*SQUARE_WIDTH, 2*SQUARE_WIDTH},
		{-2*SQUARE_WIDTH, 2*SQUARE_WIDTH, -2*SQUARE_WIDTH, -2*SQUARE_WIDTH},
		{-2*SQUARE_WIDTH, -2*SQUARE_WIDTH, 2*SQUARE_WIDTH, -2*SQUARE_WIDTH},
		{2*SQUARE_WIDTH, -2*SQUARE_WIDTH, 2*SQUARE_WIDTH, 2*SQUARE_WIDTH}};


// Square Structure for the 4 squares of each block
struct Square
{
	int m_rotIndex;	// the rotational index.  indexes into the 
					//ROTATION_INDEX array for the rotation values
	BOB m_bob; 
	int m_row;
   int m_col;	// keeps track of the adjusted row/col of the square
};

class Logger;

class Block
{
public:
   // CONSTRUCTORS
	Block( const Logger& logger );   // does not own the logger
	Block( const Block& block );     
	~Block();

	void MoveBlock( int x_direction, int y_direction, int xv, int yv );

   // rotates this block about its center square.
	void RotateBlock( RotDirection direction );

   // Block creation functions
	void CreateBlock( BlockType type, BOB_PTR block,int x, int y);
	void ReloadBlock( BlockType type, BOB_PTR block, int x, int y );

   // flips to the next graphic in the current animation sequence
	void Animate();

   // renders the block to the screen
	void Draw();

   // getters/setters for block center (upper left of center square)
	void SetX(int newX){m_x = newX;}
	void SetY(int newY){m_y = newY;}
	int GetX(){return m_x;}
	int GetY(){return m_y;}

	BlockType GetBlockType(){return m_type;}
	void SetSquareX(int i, int x){m_squares[i].m_bob.x = (float)x;}
	void SetSquareY(int i, int y){m_squares[i].m_bob.y = (float)y;}
	int GetSquareX(int i){return (int)m_squares[i].m_bob.x;}
	int GetSquareY(int i){return (int)m_squares[i].m_bob.y;}
	int GetRow(int i){return m_squares[i].m_row;}
	int GetCol(int i){return m_squares[i].m_col;}
	BOB_PTR GetBOB(int i){return &m_squares[i].m_bob;}
	int GetSquareType(int i){return m_squares[i].m_rotIndex;}
	int GetWallProximity(int direction);
	void ColumnCorrection();
	void EmptyThisRow(int y);
	Block* DoSplit(int y);
	bool IsSplitNeeded(int y);
	bool IsEmpty();
	bool IsSquareEmpty(int i){return bool(m_squares[i].m_rotIndex == EMPTY);}

private:
   const Logger& m_logger;
	Square m_squares[MAX_SQUARES_PER_BLOCK];
	int m_x; // center of block (the center of the center square)
   int m_y;	// used to move around block.					
	BlockType m_type;  // the type of block 

   int GetGeneralSquareType(int square);
	void TrackRowColumn();
	void FillToType( BlockType type );
	void SetSquareType(int i, int rot_index){m_squares[i].m_rotIndex = rot_index;}
	void SetAnimation(int index);
};

typedef Block* BLOCK_PTR;
#endif // BLOCK_H