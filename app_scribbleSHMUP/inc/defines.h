#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "cdhBase.h"
#include <math.h>
#include <stdlib.h>

#define GRAVITY Vector3(0.0f, -9.0f, 0.0f)

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1200
#define SCREEN_X 0
#define SCREEN_Y 0

#define SLEEP_TIME 30

//MACROS
#define RAD2DEG(angle) ((angle) * (180 / PI))
#define DEG2RAD(angle) ((angle) * (PI / 180))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

namespace CDH
{
	const CHUint TEAM_NEUTRAL = 0;
}

#endif // DEFINITIONS_H