#include "defines.h"
#include "graveyard.h"

float GraveyardBounds::backWallZ = GRAVEYARD_BACKWALL;
float GraveyardBounds::frontWallZ = GRAVEYARD_FRONTWALL;
float GraveyardBounds::leftWallX = GRAVEYARD_LEFTWALL;
float GraveyardBounds::rightWallX = GRAVEYARD_RIGHTWALL;
Vector3 GraveyardBounds::farLeft(leftWallX, 0.0f, backWallZ);
Vector3 GraveyardBounds::farRight(rightWallX, 0.0f, backWallZ);
Vector3 GraveyardBounds::nearLeft(leftWallX, 0.0f, frontWallZ);
Vector3 GraveyardBounds::nearRight(rightWallX, 0.0f, frontWallZ);

