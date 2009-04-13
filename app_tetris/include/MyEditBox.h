#pragma once

const int MAX_SIZE = 9;

class CMyEditBox
{
public:
	CMyEditBox(void);
	~CMyEditBox(void);
	void AddChar(char Char);
	void DeleteCurrentChar();	// deletes char at cursor
	void DeletePreviusChar();	// deletes char previous to cursor
	void MoveCursorLeft();		// moves the cursor left once
	void MoveCursorRight();
	char* GetString(){return _string;}
	int GetCursorLocation(){return _cursorLocation;}
	int GetSize(){return _size;}
private:
	char _string[MAX_SIZE];
	int _size;

	int _cursorLocation;		//0-16
};
