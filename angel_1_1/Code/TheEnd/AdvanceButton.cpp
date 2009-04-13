#include "stdafx.h"

#include "TextActor.h"
#include "World.h"
#include "AdvanceButton.h"
#include "LifeChoicePoints.h"
#include "TheEndGameManager.h"

AdvanceButton* AdvanceButton::s_advanceButton = NULL;
AdvanceButton& AdvanceButton::GetInstance()
{
   if (s_advanceButton == NULL)
   {
	   s_advanceButton = new AdvanceButton();
   }
   return *s_advanceButton;
}


AdvanceButton::AdvanceButton()
{
	m_text = new TextActor( "ConsoleSmall", "blah" );
	m_text->SetColor(Color3f(1,1,1));
	m_text->SetAlignment(TXT_Center);
}

void AdvanceButton::Update(float dt)
{
   char points[256]; 
	if(thePointTracker.getAvailablePoints() > 0)
		sprintf(points, "%d", thePointTracker.getAvailablePoints());
	else
		sprintf(points, "%s", "Advance..." ); 
   m_text->SetDisplayString(points);   
}

void AdvanceButton::SetPosition(float x, float y)
{
	m_text->SetPosition(x,y);
   Actor::SetPosition(x,y);
}

static bool g_mouseOn = false;
void AdvanceButton::AddToWorld()
{
   g_mouseOn = true;
   theWorld.Add(this, 0);
   theWorld.Add(m_text, 1);
}
void AdvanceButton::RemoveFromWorld()
{
   g_mouseOn = false;
   theWorld.Remove(this);
   theWorld.Remove(m_text);
}

void AdvanceButton::MouseMotionEvent(Vec2i screenCoords)
{
}

void AdvanceButton::MouseDownEvent(Vec2i screenCoords, MouseButtonInput button)
{
   if(g_mouseOn)
   {
      const Vector2 screenPos = MathUtil::ScreenToWorld(screenCoords.X, screenCoords.Y);
	   {
		   const Vector2 size = GetSize();
		   const Vector2 position = GetPosition();
   	   
		   float lowerX = position.X - (0.5f * size.X);
		   float upperX = position.X + (0.5f * size.X);
		   float lowerY = position.Y - (0.5f * size.Y);
		   float upperY = position.Y + (0.5f * size.Y);

		   if(screenPos.X >= lowerX && screenPos.X <= upperX &&
			   screenPos.Y >= lowerY && screenPos.Y <= upperY )
		   {
			   if( thePointTracker.getAvailablePoints() == 0 )
			   {
               thePointTracker.finalizePoints();
				   theEndGame.MoveForwards(String());
			   }			
		   }
	   }
   }
}