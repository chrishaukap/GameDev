#ifndef GLOBALS_H
#define GLOBALS_H

#include "Logger.h"

// DEFINES ////////////////////////////////////////////////
// HAUKAP - these should go in "lib_base"
typedef unsigned short int uint16; 
typedef unsigned long int uint32; 
typedef short int int16; 
typedef long int int32; 

// defines used by cfgmanager
#define MAX_LEVEL 10
#define MIN_LEVEL 0 // HAUKAP - kinda lame...
#define MAX_EFFECTS_VOLUME 100
#define MIN_EFFECTS_VOLUME 0
#define MAX_MUSIC_VOLUME 100
#define MIN_MUSIC_VOLUME 0
#define WINDOWED 1   // don't change this
#define FULLSCREEN 0 // don't change this
#define FORCE_WINDOWED 0 // 1 in debug mode to force windowed mode

// defines for windows 
#define WINDOW_CLASS_NAME "WINXCLASS"  // class name


// setup a 640x480 16-bit windowed mode example
#define WINDOW_TITLE      "tetrix Game!!"
#define WINDOW_WIDTH      640      // size of window
#define WINDOW_HEIGHT     480

#define WINDOW_BPP        32    // bitdepth of window (8,16,24 etc.)
                                // note: if windowed and not
                                // fullscreen then bitdepth must
                                // be same as system bitdepth
                                // also if 8-bit the a pallete
                                // is created and attached

const LogLevel g_logLevel = LOG_DEBUG3;
const char g_logFile[] = "log.txt"; // "f:\\tree.mine\\app_tetris\\mylog.txt";

// the starting position of the blocks (x,y)
#define GAME_ORIGIN_X		185
#define GAME_ORIGIN_Y		53
#define START_X 305
#define START_Y 13
#define START_PREVIEW_X 50 
#define START_PREVIEW_Y 350

// the number of colors available for the blocks
#define MAX_COLORS 7

// the direction multiplier. used to move the block
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define NO_CHANGE 0

// game dimension defines
#define GAME_WIDTH 260
#define GAME_HEIGHT 380
#define NUM_X_POSITIONS 13
#define NUM_Y_POSITIONS 19

// collision types defines
#define WALL_WEST 1
#define WALL_EAST 2
#define WALL_SOUTH 3
#define COLLECTION 4
#define WALL_NORTH 5
#define COLLECTION_LEFT 6
#define COLLECTION_RIGHT 7
#define NO_COLLISION 0
#define NULL 0


// various speeds
const int SPEED[10]={1,1,2,2,3,3,4,4,5,6};
const int LEVEL[10]={1000,2500,5000,10000,15000,25000,50000,75000,100000,250000};

// directions for rotation, clockwise
// and counter clockwise
enum RotDirection
{
   CW = 0,
   CCW = 2
};


const int BASE_VALUE = 10;
const int POINT_VALUES[7] = {	BASE_VALUE, BASE_VALUE, BASE_VALUE,
								BASE_VALUE, BASE_VALUE, BASE_VALUE, BASE_VALUE};


// the number of squares in a single block
const int MAX_SQUARES_PER_BLOCK =4;


// constant speeds
const int ANIMATION_SPEED = 5;
const int NUM_REG_FRAMES = 10;
const int NUM_TRANS_FRAMES = 6;
const int NUM_FLASH_FRAMES = 10;


namespace WALLS
{
	const int north = GAME_ORIGIN_Y;
	const int south = GAME_ORIGIN_Y + GAME_HEIGHT;
	const int west = GAME_ORIGIN_X;
	const int east = GAME_ORIGIN_X + GAME_WIDTH;
};

#endif //GLOBALS_H