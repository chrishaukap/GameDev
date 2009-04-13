#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <math.h>
#include <stdlib.h>

#define GRAVITY Vector3(0.0f, -9.0f, 0.0f)

#define BEGIN_PAUSED false
//#define TEST_CODE
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1200
#define SCREEN_X 0
#define SCREEN_Y 0

// OPTIMIZE hardcode
#define PI 3.141592f
#define PIOver2 1.570796f
#define PIOver3 1.047197f
#define PIOver4 0.785398f
#define PIOver5 0.628318f
#define PIOver6 0.523598f
#define PIOver7 0.448798f
#define PIOver8 0.392699f
#define PIOver9 0.349065f
#define PIOver16 0.1963495
#define PIOver32 0.09817475
#define PIOver64 0.049087375
#define PIOver128 0.0245436875
#define PIOver256 0.01227184375
#define TWOPI 6.283184


// graveyard bounds
#define GRAVEYARD_BACKWALL -20.0f
#define GRAVEYARD_FRONTWALL 20.0f
#define GRAVEYARD_LEFTWALL -20.0f
#define GRAVEYARD_RIGHTWALL 20.0f
#define GRAVEYARD_WIDTH (GRAVEYARD_RIGHTWALL - GRAVEYARD_LEFTWALL)
#define GRAVEYARD_LENGTH (GRAVEYARD_FRONTWALL - GRAVEYARD_BACKWALL)

#define SLEEP_TIME 30

// branch 
#define BRANCHNODE_WEIGHT 0.1f
#define BRANCH_GROWTH_RATE 0.32f 
#define NEW_BRANCH_LENGTH 0.1f   
#define MAX_BRANCH_LENGTH 0.5f   
#define MIN_BRANCH_WIDTH 0.1f
#define NEW_BRANCH_WIDTH MIN_BRANCH_WIDTH
#define CHANCE_NEW_GROWTH 9      // (out of 10) controls how often new growth starts on the main trunk 
#define MAX_NEW_GROWTH 3         // keep this small.  best not to play with it!
#define RENDER_BRANCHES

// LIMBS
#define MAX_LIMB_LENGTH 1.0f
#define MAX_LIMBNODE_ANGLE_PER_FRAME 1.5 * PIOver32
#define LIMB_GROWTH_ANGLE_FUDGE 0.3
#define MAX_LIMB_NODES 30
#define BRANCH_MAX_WEIGHT 1.0f
#define LIMB_WIDTH_GROWTH 0.01f
#define MAX_LIMB_WIDTH 0.35f
#define MAX_LIMBS 3
#define APPLIED_VELOCITY_MULT 10.0f

// camera
#define CAMERA_ORBIT_SPEED 0.0125f
#define CAMERA_IS_ORBITING false
#define CAMERA_LOCATION Vector3(0.0f, 5.0f, 50.0f)

// peeps
#define MAX_PEEPS 15
#define PEEPS_MOVING
#define PEEP_BOUNDING_HALFWIDTH 0.75f
#define PEEP_START_POSITION Vector3(0.0f, 0.0f, 0.0f)
#define PEEP_START_ORIENTATION Vector3(1.0f,0.0f,-1.0f)
#define PEEP_START_ROTATION Vector3(0.0f,0.0f,0.0f)
#define PEEP_SCALE 1.0f
#define PEEP_SPEED 0.3f
#define PEEP_DEAD_SPEED 1.0f // speed should be a member of peep
#define DEAD_PEEP_DURATION 50
//#define DRAW_PEEPS_BBOX
#define DEAD_PEEP_EYESCALE 0.3f
#define DEAD_PEEP_MOUTHSCALE 0.5f
#define BLOOD_SPURT_DENSITY Sparse
#define BLOOD_DROPLET_DURATION 30
#define BLOOD_DROPLET_SIZE 0.05f

// DEBUG
#if _DEBUG
#define PLAY_SOUND true
#else
#define PLAY_SOUND false
#endif

//MACROS
#define RAD2DEG(angle) ((angle) * (180 / PI))
#define DEG2RAD(angle) ((angle) * (PI / 180))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif // DEFINITIONS_H