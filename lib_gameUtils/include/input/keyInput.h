#ifndef KEYINPUT_H
#define KEYINPUT_H

#include "cdhBase.h"



namespace CDH
{

enum SpecialKeys
{
   BeginArrowKeys = 37,
   LeftKey = 37, UpKey, RightKey, DownKey,
   EndArrowKeys = 41
};

// this enum indexes directly into KeyInput::m_keys[]
enum KeyIndex{
Idx_A= 0, BeginAlpha = Idx_A, Idx_B, Idx_C, Idx_D, Idx_E, Idx_F, Idx_G, Idx_H, Idx_I, Idx_J, Idx_K,
Idx_L, Idx_M, Idx_N, Idx_O, Idx_P, Idx_Q, Idx_R, Idx_S, Idx_T, Idx_U, Idx_V,
Idx_W, Idx_X, Idx_Y, Idx_Z, 
Idx_0, BeginNum = Idx_0, Idx_1, Idx_2, Idx_3, Idx_4, Idx_5, Idx_6, Idx_7, Idx_8, Idx_9, 
Idx_LEFT, BeginArrows = Idx_LEFT, Idx_UP, Idx_RIGHT, Idx_DOWN, 
Num_Idxs
};

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	bool keyPressed(KeyIndex key) const;
   bool keyPressed(int key) const;


	bool processKeyDown(int key);
	bool processKeyUp(int key);
private:	
	bool m_keys[Num_Idxs];
	void setKeyPressed(KeyIndex key, bool pressed);
   bool processKey(int key, bool keyPressed);
   
   KeyIndex windowsKeyToKeyIndex(int key) const;

   KeyInput(const KeyInput& C);
   const KeyInput& operator=(const KeyInput& C);
};

}


#endif // KEYINPUT_H