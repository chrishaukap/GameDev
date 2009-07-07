#include "StdAfx.h"
#include "Input.h"
#include "Camera.h"
#include "World.h"
#include "Console.h"
#include "InputManager.h"


void keyboardDown(unsigned char key, int x, int y) 
{
	if( theConsole.GetInput(key) )
		return;

	if( theInput.OnKeyDown((int)key))
		return;

	if( key == theConsole.GetToggleConsoleKey() )
		theConsole.Enable();

	if (key == VK_ESCAPE) 
	{
		theWorld.StopGame();
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	if( theConsole.IsEnabled() )
		return;

	if( theInput.OnKeyUp((int)key))
		return;

}

void keyboardSpecialDown(int key, int x, int y) 
{
	if( theConsole.GetSpecialInputDown(key) )
		return;
}

void keyboardSpecialUp(int key, int x, int y) 
{
}