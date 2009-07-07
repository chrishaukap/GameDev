#include "StdAfx.h"
#include "Xbox360Input.h"
#include "InputManager.h"

Xbox360ControllerManager* Xbox360ControllerManager::s_Xbox360ControllerManager = NULL;

Xbox360ControllerManager& Xbox360ControllerManager::GetInstance()
{
	if (s_Xbox360ControllerManager == NULL)
	{
		s_Xbox360ControllerManager = new Xbox360ControllerManager();
	}
	return *s_Xbox360ControllerManager;
}

Xbox360ControllerManager::Xbox360ControllerManager()
{
	Xbox360Controller* pNewController;

	_controllers[0] = new Xbox360Controller();
	_controllers[0]->_controllerID = 0;

	_controllers[1] = new Xbox360Controller();
	_controllers[1]->_controllerID = 1;
}

void Xbox360ControllerManager::Setup()
{
	_controllers[0]->Setup();
	_controllers[1]->Setup();
}

void Xbox360ControllerManager::UpdateState()
{	
	_controllers[0]->UpdateState();
	_controllers[1]->UpdateState();
}

Xbox360Controller* Xbox360Controller::s_Xbox360Controller = NULL;

Xbox360Controller::Xbox360Controller()
{
	_threeSixtyConnected = false;
	_currentLeftVibration = 0;
	_currentRightVibration = 0;
}

Xbox360Controller& Xbox360Controller::GetInstance()
{
	if (s_Xbox360Controller == NULL)
	{
		s_Xbox360Controller = new Xbox360Controller();
	}
	return *s_Xbox360Controller;
}

void Xbox360Controller::Setup()
{
	ZeroMemory( &_currentControllerRawState, sizeof(XINPUT_STATE));

	_dwResult = XInputGetState(0, &_currentControllerRawState);

	if (_dwResult == ERROR_SUCCESS)
	{
		std::cout << "Xbox360 Controller connected!\n";
		_threeSixtyConnected = true;
	}
	else
	{
		std::cout << "No Xbox360 Controller present...\n";
		_threeSixtyConnected = false;
	}
}

void Xbox360Controller::UpdateState()
{
	_dwResult = XInputGetState(_controllerID, &_currentControllerRawState);

	// We either don't have a controller or the Input has failed.  Keep clearing the input
	// data so we don't get garbage input.
	if (_dwResult != ERROR_SUCCESS)
	{
		if (_threeSixtyConnected)
		{
			std::cout << "Uh oh, we lost the 360 Controller..." << std::endl;
		}

		_threeSixtyConnected = false;
		ZeroMemory( &_currentControllerRawState, sizeof(XINPUT_STATE));
		_currentControllerInput.LeftThumbstickX = 
			_currentControllerInput.LeftThumbstickY =
			_currentControllerInput.RightThumbstickX =
			_currentControllerInput.RightThumbstickY = 
			_currentControllerInput.LeftTriggerValue = 
			_currentControllerInput.RightTriggerValue = 
			_currentControllerInput.Buttons = 0;
		return;
	}

	_threeSixtyConnected = true;

	_currentControllerInput.Buttons = _currentControllerRawState.Gamepad.wButtons;
	//TODO: dpad
	_currentControllerInput.LeftTriggerValue = (int)_currentControllerRawState.Gamepad.bLeftTrigger;
	_currentControllerInput.RightTriggerValue = (int)_currentControllerRawState.Gamepad.bRightTrigger;
	_currentControllerInput.LeftThumbstickX = _currentControllerRawState.Gamepad.sThumbLX;
	_currentControllerInput.LeftThumbstickY = _currentControllerRawState.Gamepad.sThumbLY;
	_currentControllerInput.RightThumbstickX = _currentControllerRawState.Gamepad.sThumbRX;
	_currentControllerInput.RightThumbstickY = _currentControllerRawState.Gamepad.sThumbRY;

	//Using the built-in deadzone -- comment out if you don't like it
	if (_currentControllerInput.LeftThumbstickX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		_currentControllerInput.LeftThumbstickX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		_currentControllerInput.LeftThumbstickY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		_currentControllerInput.LeftThumbstickY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		_currentControllerInput.LeftThumbstickX = 0;
		_currentControllerInput.LeftThumbstickY = 0;
	}
	if (_currentControllerInput.RightThumbstickX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		_currentControllerInput.RightThumbstickX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		_currentControllerInput.RightThumbstickY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		_currentControllerInput.RightThumbstickY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		_currentControllerInput.RightThumbstickX = 0;
		_currentControllerInput.RightThumbstickY = 0;
	}

	theInput.HandleXboxControl( *this );
}

const ControllerInput Xbox360Controller::GetState()
{
	return _currentControllerInput;
}

const Vector2 Xbox360Controller::GetLeftThumbVec2()
{
	Vector2 forReturn;
	forReturn.X = float(_currentControllerInput.LeftThumbstickX) / 32768.0f;
	forReturn.Y = float(_currentControllerInput.LeftThumbstickY) / 32768.0f;
	return forReturn;
}

const Vec2i Xbox360Controller::GetLeftThumbstick()
{
	Vec2i forReturn;
	forReturn.X = _currentControllerInput.LeftThumbstickX;
	forReturn.Y = _currentControllerInput.LeftThumbstickY;
	return forReturn;
}
const Vec2i Xbox360Controller::GetRightThumbstick()
{
	Vec2i forReturn;
	forReturn.X = _currentControllerInput.RightThumbstickX;
	forReturn.Y = _currentControllerInput.RightThumbstickY;
	return forReturn;
}

const int Xbox360Controller::GetRightTrigger()
{
	return _currentControllerInput.RightTriggerValue;
}

const int Xbox360Controller::GetLeftTrigger()
{
	return _currentControllerInput.LeftTriggerValue;
}

const bool Xbox360Controller::IsButtonDown(int buttonMask)
{
	return ((_currentControllerInput.Buttons & buttonMask) != 0);

}

const bool Xbox360Controller::IsAButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsBButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_B)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsXButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_X)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsYButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsLeftThumbstickButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsRightThumbstickButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsStartButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_START)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsBackButtonDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_BACK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsLeftBumperDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsRightBumperDown()
{
	if(_currentControllerInput.Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsLeftTriggerPressed()
{
	if (_currentControllerRawState.Gamepad.bLeftTrigger && 
		_currentControllerRawState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Xbox360Controller::IsRightTriggerPressed()
{
	if (_currentControllerRawState.Gamepad.bRightTrigger && 
		_currentControllerRawState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const unsigned int Xbox360Controller::GetLeftVibration()
{
	return _currentLeftVibration;
}

const unsigned int Xbox360Controller::GetRightVibration()
{
	return _currentRightVibration;
}

void Xbox360Controller::SetLeftVibrationRaw(unsigned int newVibration)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = _currentLeftVibration = newVibration;
	vibration.wRightMotorSpeed = _currentRightVibration;
	XInputSetState(0, &vibration);
}

void Xbox360Controller::SetRightVibrationRaw(unsigned int newVibration)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = _currentLeftVibration;
	vibration.wRightMotorSpeed = _currentRightVibration = newVibration;
	XInputSetState(0, &vibration);
}

void Xbox360Controller::SetLeftVibration(unsigned int newVibration)
{
	newVibration = MathUtil::Clamp(newVibration, 0, 255);
	if (newVibration == 255)
	{
		SetLeftVibrationRaw(65535);
	}
	else
	{
		SetLeftVibrationRaw(newVibration << 8);
	}
}

void Xbox360Controller::SetRightVibration(unsigned int newVibration)
{
	newVibration = MathUtil::Clamp(newVibration, 0, 255);
	if (newVibration == 255)
	{
		SetRightVibrationRaw(65535);
	}
	else
	{
		SetRightVibrationRaw(newVibration << 8);
	}
}