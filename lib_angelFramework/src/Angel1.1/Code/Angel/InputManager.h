#pragma once
#include "Console.h"
#include "StringUtil.h"
#include <hash_map>

class InputBinding
{
public:

	void SetKeyDownCommand( const String& keyDownCommand );
	void SetKeyUpCommand( const String& keyUpCommand );

	void OnKeyDown();
	void OnKeyUp();

private:
	String _keyDownCommand;
	String _keyUpCommand;
};

enum InputKeys
{
#define USEINPUTKEY(e,hashVal,text) \
	e=hashVal,
#include "InputKeys.h"
#undef USEINPUTKEY
};

#define theInput InputManager::GetInstance()

class InputManager : public CCmdMemberFunctorOwner
{
	typedef stdext::hash_map<int, InputBinding*>		BindingTable;
	typedef stdext::hash_map<String, int>					KeyNameTable;
	typedef stdext::hash_map<int, bool>					XboxButtonState;
public:
	static InputManager &GetInstance();
	static void Destroy();

	void BindKey( const String& keyId, const String& command );
	void UnbindKey( const String& keyId );

	bool OnKeyDown( int keyVal );
	bool OnKeyUp( int keyVal );

	void HandleXboxControl( class Xbox360Controller& controller );

protected:
	InputManager() {}
	~InputManager();
	void Initialize();

private:
	InputBinding* GetBinding( int hashVal );
	int GetHashFromKeyName( const String& keyId );
	void BindKeyCommand( const String& input );
	void UnbindKeyCommand( const String& input );
	void ClearXboxButtonStates();

private:
	static InputManager* s_Input;

	KeyNameTable		_keyNameTable;
	BindingTable		_bindingTable;
	XboxButtonState		_xBoxButtonStates;
};