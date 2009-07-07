#include "stdafx.h"
#include "MathUtil.h"
#include "Camera.h"
#include <math.h>
#include <time.h>

const float MathUtil::E = 2.718282f;
const float MathUtil::Log10E = 0.4342945f;
const float MathUtil::Log2E = 1.442695f;
const float MathUtil::Pi = 3.141593f;
const float MathUtil::PiOver2 = 1.570796f;
const float MathUtil::PiOver4 = 0.7853982f;
const float MathUtil::TwoPi = 6.283185f;
const float MathUtil::MaxFloat = 3.402823E+38f;
const float MathUtil::MinFloat = -3.402823E+38f;
const float MathUtil::Epsilon = 0.000001f;


#include <iostream>
using std::cout;
using std::endl;
void MathUtil::UnitTest()
{
	cout << "Max: " << Max(10, 200) << " " << Max(0.0f, -192.0f) << endl;
	cout << "Min: " << Min(10, 200) << " " << Min(0.0f, -192.0f) << endl;
	cout << "Clamp: " << Clamp(10, 200, 1000) << " " << Clamp(0.0f, -192.0f, -1.0f) << endl;

	for ( int i = 0; i <= 10; i++ )
	{
		float val = (float)i * 0.1f;
		cout << SmoothStep(2, 12, val ) << endl; 
	}

}

float MathUtil::SmoothStep(float value1, float value2, float amount)
{
	float num = Clamp(amount, 0.0f, 1.0f);
	return Lerp(value1, value2, (num * num) * (3.0f - (2.0f * num)));
}

float MathUtil::ToDegrees(float radians)
{
	return (radians * 57.29578f);
}

float MathUtil::ToRadians(float degrees)
{
	return (degrees * 0.01745329f);
}

double MathUtil::DistanceSquared(double x1, double y1, double x2, double y2) 
{
	return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

double MathUtil::Distance(double x1, double y1, double x2, double y2) 
{
	return sqrt(DistanceSquared(x1,y1,x2,y2));
}

Vector2 MathUtil::VectorFromAngle(float angle_in_degrees)
{
	return Vector2(cos(ToRadians(angle_in_degrees)), sin(ToRadians(angle_in_degrees)));
}

float MathUtil::AngleFromVector(const Vector2& v1)
{
	Vector2 localVector = v1;

	localVector.Normalize();
	return atan( localVector.Y / localVector.X );	
}

float MathUtil::AngleFromVectors(const Vector2& v1, const Vector2& v2)
{	
	return atan2(v1.Y, v1.X) - atan2(v2.Y, v2.X);
}


int MathUtil::RoundToInt(double x)
{
	return ((int)(floor(x+0.5)));
}

int MathUtil::RandomInt(int maximum)
{
	static bool firstTime = true;
	if (firstTime) 
	{
		firstTime = false;
		srand((int)time(NULL));
	}
	if (maximum <= 0) 
	{
		return 0;
	}
	return (rand() % maximum);
}

int MathUtil::RandomIntInRange(int min, int max)
{
	return RandomInt(max-min) + min;
}

int MathUtil::RandomIntWithError(int target, int error)
{
	return RandomIntInRange(target-error, target+error);
}

float MathUtil::RandomFloat(float maximum)
{
	const float bigNumber = 10000.0f;
	float randFloat = (float)RandomInt((int)bigNumber);
	randFloat = randFloat/bigNumber;
	return randFloat*maximum;
}

float MathUtil::RandomFloatInRange(float min, float max)
{
	return RandomFloat(max-min) + min;
}

float MathUtil::randomizeValue( float value, float fudge )
{
   // amount of total fudge factor
   double randPercent = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
   float dAngle = (float)randPercent * fudge;
   
   // + or - fudge factor
   int plusOrMinus = 0;
   plusOrMinus = rand()%2;
   if(plusOrMinus == 0)
      return value + dAngle;
   else
      return value - dAngle;
}

float MathUtil::RandomFloatWithError(float target, float error)
{
	return RandomFloatInRange(target-error, target+error);
}

bool MathUtil::RandomBool()
{
	return (bool)MathUtil::RandomInt(2);
}

Vector2 MathUtil::RandomVector()
{
	return RandomVector(Vector2(1.0f));//.Normalize();
}

Vector2 MathUtil::RandomVector(Vector2 maxValues)
{
	return RandomVector(Vector2::Zero, maxValues);
}

Vector2 MathUtil::RandomVector(Vector2 minValues, Vector2 maxValues)
{
	return Vector2(RandomFloatInRange(minValues.X, maxValues.X), RandomFloatInRange(minValues.Y, maxValues.Y));
}

bool MathUtil::FuzzyEquals(float x, float y, float epsilon)
{
	float a = x - y;
	if (abs(a) < epsilon)
	{
		return true;
	}
	return false;
}


bool MathUtil::FuzzyEquals(Vector2 v1, Vector2 v2, float epsilon)
{
	if ( (MathUtil::FuzzyEquals(v1.X, v2.X, epsilon)) && (MathUtil::FuzzyEquals(v1.Y, v2.Y, epsilon)) )
	{
		return true;
	}
	return false;
}

Vector2 MathUtil::ScreenToWorld(Vec2i screenCoordinates)
{
	return MathUtil::ScreenToWorld(screenCoordinates.X, screenCoordinates.Y);
}

Vector2 MathUtil::ScreenToWorld(int x, int y)
{
	float worldWidth, worldHeight;
	int screenWidth = theCamera.GetWindowWidth();
	int screenHeight = theCamera.GetWindowHeight();
	float aspect = (float)screenWidth / (float)screenHeight;
	if (screenWidth > screenHeight)
	{
		//window is wider than it is tall; radius goes with height
		worldHeight = theCamera.GetViewRadius() * 2.0f;
		worldWidth = worldHeight * aspect;
	}
	else
	{
		//window is taller than it is wide; radius goes with width
		worldWidth = theCamera.GetViewRadius() * 2.0f;
		worldHeight = worldWidth / aspect;
	}

	float worldX = ( ((float)x / (float)screenWidth) - 0.5f ) * worldWidth;
	float worldY = ( 0.5f - ((float)y / (float)screenHeight) ) * worldHeight;
	
	Vector2 camPos = theCamera.GetPosition();
	return Vector2(worldX + camPos.X, worldY + camPos.Y);
}

Vector2 MathUtil::WorldToScreen(Vector2 worldCoordinates)
{
	return WorldToScreen(worldCoordinates.X, worldCoordinates.Y);
}

Vector2 MathUtil::WorldToScreen(float x, float y)
{
	Vector2 camPos = theCamera.GetPosition();
	x -= camPos.X;
	y -= camPos.Y;

	float worldWidth, worldHeight;
	int screenWidth = theCamera.GetWindowWidth();
	int screenHeight = theCamera.GetWindowHeight();
	float aspect = (float)screenWidth / (float)screenHeight;
	if (screenWidth > screenHeight)
	{
		//window is wider than it is tall; radius goes with height
		worldHeight = theCamera.GetViewRadius() * 2.0f;
		worldWidth = worldHeight * aspect;
	}
	else
	{
		//window is taller than it is wide; radius goes with width
		worldWidth = theCamera.GetViewRadius() * 2.0f;
		worldHeight = worldWidth / aspect;
	}

	float screenX = screenWidth * ( (x / worldWidth) + 0.5f );
	float screenY = screenHeight - (screenHeight * ( 0.5f + (y / worldHeight) ));

	return Vector2(screenX, screenY);
}

MathUtil::AABBSplittingAxis MathUtil::GetMajorAxis(const BoundingBox& source)
{
	AABBSplittingAxis retVal = AA_X;
	float maxAxis = source.Max.X - source.Min.X;

	float yAxis = source.Max.Y - source.Min.Y;
	if (yAxis > maxAxis)
	{
		retVal = AA_Y;
		maxAxis = yAxis;
	}

	return retVal;
}

void MathUtil::SplitBoundingBox(const BoundingBox& source, AABBSplittingAxis axis, BoundingBox& LHS, BoundingBox& RHS)
{
	LHS = source;
	RHS = source;

	switch (axis)
	{
	case AA_X:
		LHS.Max.X = Lerp(LHS.Min.X, LHS.Max.X, 0.5f);
		RHS.Min.X = LHS.Max.X;
		break;
	case AA_Y:
		LHS.Max.Y = Lerp(LHS.Min.Y, LHS.Max.Y, 0.5f);
		RHS.Min.Y = LHS.Max.Y;
		break;
	}
}

float MathUtil::DeltaAngle(float A1, float A2)
{
	// Find the difference
	float Delta = A2 - A1;

	// If change is larger than PI
	if(Delta > Pi)
	{
		// Flip to negative equivalent
		Delta = Delta - (TwoPi);
	}
	else if(Delta < -Pi)
	{
		// Otherwise, if change is smaller than -PI
		// Flip to positive equivalent
		Delta = Delta + (TwoPi);
	}

	// Return delta in [-PI,PI] range
	return Delta;
}

float MathUtil::VectorDeltaAngle(Vector2 v1, Vector2 v2)
{
	return acos( Vector2::Dot(v1, v2) );
}


Vector2 MathUtil::MoveP1TowardP2(const Vector2& pToMove, 
											const Vector2& dstPoint, 
											float speed)
{
   Vector2 vecV1ToV2 = dstPoint - pToMove;
   vecV1ToV2.Normalize();
      
   // move v1 toward v2 by speed
   return Vector2( pToMove + (vecV1ToV2 * speed) );	
}