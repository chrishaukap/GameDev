#include "stdafx.h"
#include "DemoScreenConsole.h"

#include "World.h"
#include "ActorFactory.h"
#include "StringUtil.h"


Actor* localActor = NULL;

void AddTexture(const String& input)
{
	localActor->SetSprite("Resources\\Images\\angel.png");
}

DemoScreenConsole::DemoScreenConsole()
{
}

void DemoScreenConsole::Start()
{
	//Place the actor from a definition file
	localActor = a = theActorFactory.CreateActor("simple_actor", "",	0, NULL);
	
	//Declare a console command for a static function
	//  (note the function must return void and take a constant string as input
	CONSOLE_DECLARECMDSTATIC(
		Add_Texture,	//What you'll put in the command line (will be uppercased in the console)
		AddTexture		//The function to call
	);
	
	//Declare a console command for an object method
	CONSOLE_DECLARECMDMETHOD(
		Change_Size,		//the command to enter
		this,				//the object you want to access
		DemoScreenConsole,	//the class of the object you want to access
		ChangeSize			//the function to call (same signature rules as above
	);

	//NOTE: DemoScreenConsole, in this case, has to inherit from CCmdMemberFunctorOwner
	//  so that it can store the console commands. 







	//Demo housekeeping below this point. 
	#pragma region Demo housekeeping
	t = new TextActor("Console", "This demo shows off the console.");
	t->SetPosition(0, -3.5f);
	t->SetAlignment(TXT_Center);
	t2 = new TextActor("Console", "Press ~ to open it up. Execute \"ADD_TEXTURE\", enjoying the tab-completion.");
	t2->SetPosition(0, -4.5f);
	t2->SetAlignment(TXT_Center);
	t3 = new TextActor("Console", "Then try executing \"CHANGE_SIZE 3.14\" or whatever number suits your fancy.");
	t3->SetPosition(0, -5.5f);
	t3->SetAlignment(TXT_Center);
	theWorld.Add(t);
	theWorld.Add(t2);
	theWorld.Add(t3);
	TextActor *fileLoc = new TextActor("ConsoleSmall", "DemoScreenConsole.cpp, simple_actor.adf");
	fileLoc->SetPosition(MathUtil::ScreenToWorld(5, 763));
	fileLoc->SetColor(.3f, .3f, .3f);
	theWorld.Add(fileLoc);
	_objects.push_back(fileLoc);
	_objects.push_back(t);
	_objects.push_back(t2);
	_objects.push_back(t3);
	_objects.push_back(a);
	#pragma endregion
}

void DemoScreenConsole::ChangeSize(const String& input)
{
	a->SetSize(StringToFloat(input));
}