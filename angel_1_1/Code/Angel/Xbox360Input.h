#pragma once

#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

/*
// Here are the definitions from XInput.h
#define XINPUT_GAMEPAD_DPAD_UP          0x00000001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
#define XINPUT_GAMEPAD_START            0x00000010
#define XINPUT_GAMEPAD_BACK             0x00000020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/

struct ControllerInput
{
	int LeftThumbstickX;
	int LeftThumbstickY;
	int RightThumbstickX;
	int RightThumbstickY;
	int LeftTriggerValue;
	int RightTriggerValue;
	WORD Buttons;
};

class Xbox360Controller;

#define MAX_CONTROLLERS 2

class Xbox360ControllerManager
{
public:
	static Xbox360ControllerManager& GetInstance();
	Xbox360ControllerManager::Xbox360ControllerManager();

	Xbox360Controller& GetController(int controllerIndex=0)
	{
		if ( (controllerIndex >= MAX_CONTROLLERS) || (controllerIndex < 0) )
		{
			std::cout << "ERROR: Bad controller index." << std::endl;
		}
		return *_controllers[controllerIndex];
	}

	void Setup();
	void UpdateState();

protected:
	static Xbox360ControllerManager *s_Xbox360ControllerManager;

private:
	Xbox360Controller* _controllers[MAX_CONTROLLERS];
};

#define theControllerManager Xbox360ControllerManager::GetInstance()
#define theController Xbox360ControllerManager::GetInstance().GetController()

static Xbox360ControllerManager* s_controllerManager = NULL;

class Xbox360Controller  
{
public:
	friend class Xbox360ControllerManager;

	static Xbox360Controller& GetInstance();

	void Setup();
	void UpdateState();
	const ControllerInput GetState();

	const Vector2 GetLeftThumbVec2();
	const Vec2i GetLeftThumbstick();
	const Vec2i GetRightThumbstick();
	const int GetRightTrigger();
	const int GetLeftTrigger();
	const bool IsButtonDown(int buttonMask);
	const bool IsAButtonDown();
	const bool IsBButtonDown();
	const bool IsXButtonDown();
	const bool IsYButtonDown();
	const bool IsLeftThumbstickButtonDown();
	const bool IsRightThumbstickButtonDown();
	const bool IsStartButtonDown();
	const bool IsBackButtonDown();
	const bool IsLeftBumperDown();
	const bool IsRightBumperDown();
	const bool IsLeftTriggerPressed();
	const bool IsRightTriggerPressed();

	void SetLeftVibrationRaw(unsigned int vibration); //lower frequencies, 0-65535
	void SetRightVibrationRaw(unsigned int vibration); //higher frequencies, 0-65535
	void SetLeftVibration(unsigned int vibration); //lower frequencies, 0-255
	void SetRightVibration(unsigned int vibration); //higher frequencies, 0255
	const unsigned int GetLeftVibration(); 
	const unsigned int GetRightVibration();

	const bool IsConnected() { return _threeSixtyConnected; }

	int _controllerID;

protected:
	Xbox360Controller();
	static Xbox360Controller *s_Xbox360Controller;

private:
	bool _threeSixtyConnected;
	XINPUT_STATE _currentControllerRawState;
	ControllerInput _currentControllerInput;

	unsigned int _currentLeftVibration;
	unsigned int _currentRightVibration;

	DWORD _dwResult;
};
