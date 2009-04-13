#include "StdAfx.h"

#include "VineScreen.h"
#include "World.h"
#include "TextActor.h"
#include "Vine.h"

VineScreen::VineScreen(unsigned int width, unsigned int height ) : 
	m_screenWidth(width), m_screenHeight(height), m_vine(NULL)
{}
VineScreen::~VineScreen() 
{
	delete m_vine;
}

void VineScreen::Start()
{
   Vector2 fULCorner = MathUtil::ScreenToWorld(0, 0);
   Vector2 fLRCorner = MathUtil::ScreenToWorld(m_screenWidth, m_screenHeight);
   Vector2 scrnDims( fabs(fLRCorner.X) + fabs(fULCorner.X), fabs(fLRCorner.Y) + fabs(fULCorner.Y) );
	
	m_vine = createVine();
	
	TextActor *actor = new TextActor("Console", "VINE SCREEN");
	actor->SetColor(0,0,0);
	actor->SetPosition(0, 0);
	actor->SetAlignment(TXT_Center);
	//theWorld.Add(actor,3);
	m_objects.push_back(actor);
}
void VineScreen::Update(float dt)
{
	m_vine->update();
}
void VineScreen::Render() 
{
	m_vine->render();
}
void VineScreen::Stop()
{
	GameScreen::Stop();
}
Vine* VineScreen::createVine()
{
	delete m_vine;
	return new Vine();
}
