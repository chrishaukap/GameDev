#include "assert.h"
#include "input/keyInput.h"
#include <string.h>

using namespace CDH;


KeyInput::KeyInput()
{
   for(CHUint i=0; i<Num_Idxs; ++i)
      m_keys[i] = false;
}
KeyInput::~KeyInput()
{
}

KeyIndex
KeyInput::windowsKeyToKeyIndex(int key) const
{
   KeyIndex ret = Num_Idxs;

   // arrow keys
   if((SpecialKeys)key >= BeginArrowKeys && (SpecialKeys)key < EndArrowKeys)
      ret = KeyIndex(key - (int)BeginArrowKeys + BeginArrows);
   
   // alpha
   else if (key >= 'A' && key <= 'Z')
		ret = KeyIndex(key - 'A' + BeginAlpha);

   // number
   else if (key >= '0' && key <= '9')
      ret = KeyIndex(key - '0' + BeginNum);

   assert(ret >= 0 && ret < Num_Idxs);
   return ret;
}

bool 
KeyInput::keyPressed(int key) const
{
   return keyPressed(windowsKeyToKeyIndex(key));
}
bool 
KeyInput::keyPressed(KeyIndex key) const
{
	if(key >= Num_Idxs || key < 0)
		return false;

	return m_keys[key];
}


bool 
KeyInput::processKey(int key, bool keyPressed)
{
   // arrow keys
   if( (SpecialKeys)key >= BeginArrowKeys && (SpecialKeys)key < EndArrowKeys ||
       key >= 'A' && key <= 'Z' ||
       key >= '0' && key <= '9' )
   {
      setKeyPressed( windowsKeyToKeyIndex(key), keyPressed );
   }

	return false;
}

bool 
KeyInput::processKeyDown(int key)
{
   return processKey(key, true);
}
bool 
KeyInput::processKeyUp(int key)
{
   return processKey(key, false);
}

void 
KeyInput::setKeyPressed(KeyIndex key, bool pressed)
{
   assert( (CHUint)key >= 0 && (CHUint)key < Num_Idxs );
	m_keys[key] = pressed;
}