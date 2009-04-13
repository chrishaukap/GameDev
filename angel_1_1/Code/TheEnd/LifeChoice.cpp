#include "stdafx.h"

#include "world.h"
#include "LifeChoice.h"
#include "TextActor.h"
#include "LifeChoicePoints.h"

const float LifeChoice::m_actorPadding = 0.2f;

LifeChoice::LifeChoice(float x, float y, float iconWidth, float iconHeight,
							  float bttnSize, 
							  Color3f color,
							  String string) : 
	m_x(x), m_y(y),
	m_tmpPoints(0),
   m_points(0)
{   
	Color3f textcolor(1,1,1);
   m_icon = new Actor;
   m_icon->SetColor(color);
   m_icon->SetSize(iconWidth, iconHeight);

	m_iconText = new TextActor( "ConsoleSmall", string );
	m_iconText->SetColor(textcolor);
	m_iconText->SetAlignment(TXT_Center);
   m_iconText->SetSize(iconWidth, iconHeight);

   m_numPoints = new Actor;
   m_numPoints->SetColor(color);
   m_numPoints->SetSize(bttnSize);

   m_numPointsText = new TextActor( "ConsoleSmall" );
	m_numPointsText->SetColor(textcolor);
	m_numPointsText->SetAlignment(TXT_Center);
   m_numPointsText->SetSize(bttnSize);
	SetNumPointsDisplayString();

   m_addPoint = new Actor;
   m_addPoint->SetColor(color);
   m_addPoint->SetSize(bttnSize);

   m_subPoint = new Actor;
   m_subPoint->SetColor(color);
   m_subPoint->SetSize(bttnSize);

	SetPosition(m_x,m_y);
}

void LifeChoice::SetPosition(float x, float y)
{
	m_icon->SetPosition(x,y);
	m_iconText->SetPosition(x,y);
	float iconSize = m_icon->GetSize().Y;
	float bttnSize = m_addPoint->GetSize().Y;

	y -= (0.5f * iconSize) + m_actorPadding + (0.5f * bttnSize);
   m_addPoint->SetPosition(x, y);

	y -= bttnSize + m_actorPadding;
	m_numPoints->SetPosition(x, y);
	m_numPointsText->SetPosition(x,y);

	y -= bttnSize + m_actorPadding;
   m_subPoint->SetPosition(x, y);
}

void LifeChoice::AddToWorld()
{
	theWorld.Add(m_icon, 0);
	theWorld.Add(m_iconText, 1);
	theWorld.Add(m_numPoints, 0);
	theWorld.Add(m_numPointsText, 1);
	theWorld.Add(m_addPoint, 0);
	theWorld.Add(m_subPoint, 0);
}
void LifeChoice::RemoveFromWorld()
{
	theWorld.Remove(m_icon);
	theWorld.Remove(m_iconText);
	theWorld.Remove(m_numPoints);
	theWorld.Remove(m_numPointsText);
	theWorld.Remove(m_addPoint);
	theWorld.Remove(m_subPoint);
}

unsigned int 
LifeChoice::GetPoints() const
{
   return m_points;
}
void LifeChoice::SetNumPointsDisplayString()
{	
	char points[4]; // up to 999
	sprintf(points, "%d", m_points + m_tmpPoints);
   m_numPointsText->SetDisplayString(points);
}

void 
LifeChoice::SetPoints(unsigned int points)
{
	m_points = points;
   m_tmpPoints = 0;
	SetNumPointsDisplayString();
}
void LifeChoice::addPoints()
{
   if(thePointTracker.getAvailablePoints() > 0)
   {
      ++m_tmpPoints;
      thePointTracker.decrementAvailablePoints();
   }
	SetNumPointsDisplayString();
}
void LifeChoice::decPoints()
{
	if(m_tmpPoints > 0 && 
		thePointTracker.getAvailablePoints() < 
		thePointTracker.getMaxPoints() )
   {
		--m_tmpPoints;
      thePointTracker.incrementAvailablePoints();
   }
	SetNumPointsDisplayString();
}

void LifeChoice::MouseMotionEvent(Vec2i screenCoordinates)
{
   // rollovers?
}

void LifeChoice::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
   const Vector2 screenPos = MathUtil::ScreenToWorld(screenCoords.X, screenCoords.Y);

	// add button
	{
		const Vector2 size = m_addPoint->GetSize();
		const Vector2 position = m_addPoint->GetPosition();
	   
		float lowerX = position.X - (0.5f * size.X);
		float upperX = position.X + (0.5f * size.X);
		float lowerY = position.Y - (0.5f * size.Y);
		float upperY = position.Y + (0.5f * size.Y);

		if(screenPos.X >= lowerX && screenPos.X <= upperX &&
			screenPos.Y >= lowerY && screenPos.Y <= upperY)
			addPoints();
	}

	// subtract button
	{
		const Vector2 size = m_subPoint->GetSize();
		const Vector2 position = m_subPoint->GetPosition();
	   
		float lowerX = position.X - (0.5f * size.X);
		float upperX = position.X + (0.5f * size.X);
		float lowerY = position.Y - (0.5f * size.Y);
		float upperY = position.Y + (0.5f * size.Y);

		if(screenPos.X >= lowerX && screenPos.X <= upperX &&
			screenPos.Y >= lowerY && screenPos.Y <= upperY )
			decPoints();
	}

}

void 
LifeChoice::finalizePoints()
{
   m_points += m_tmpPoints;
   m_tmpPoints = 0;
}