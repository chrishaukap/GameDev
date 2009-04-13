#include "stdafx.h"
#include "CutSceneScreen.h"

#include "World.h"
#include "TextActor.h"

CutSceneScreen::CutSceneScreen()
{
}

void CutSceneScreen::Start()
{
	//Demo housekeeping below this point. 
	#pragma region Demo Housekeeping
	t = new TextActor("Console", "CutSceneScreen");
	t->SetPosition(0, 0);
	t->SetAlignment(TXT_Center);
	theWorld.Add(t);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "CutSceneScreen.cpp");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	m_objects.push_back(fileLoc);
	m_objects.push_back(t);
	#pragma endregion
}

void CutSceneScreen::Update(float dt)
{
}