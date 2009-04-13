#include "myeditbox.h"

CMyEditBox::CMyEditBox(void): _size(0), _cursorLocation(0)
{
	for( int i=0; i<MAX_SIZE; ++i )
		_string[i] = ' ';
	_string[MAX_SIZE-1] = '\0';
}

CMyEditBox::~CMyEditBox(void)
{
}

// adds Char to the string at the point specified by _cursorLocation
// then moves _cursorLocation right one space
void CMyEditBox::AddChar(char Char)
{
	int i,j;
	// check the validity of _cursorLocation first
	if( _cursorLocation < 0 || _cursorLocation >= MAX_SIZE )
		return; // _cursorLocation is out of bounds... 
	if( _size >= (MAX_SIZE -1))
		return; // no more room

	// find the first empty space to the right of the cursor
	i= _cursorLocation;
	for( ; _string[i] != '\0' && _string[i] != ' '; ++i );
	j=i;
		
	// move all characters right of cursor... right one space
	// to make room for new character
	i= _cursorLocation;
	while( j>i )
	{
		_string[j] = _string[i]; 
		--j;
	}

	// add the character at _cursorLocation
	_string[_cursorLocation] = Char;
	++_size;
	MoveCursorRight();
}

// delete the character to the right of the cursor
// shifts characters right of cursor left one space
void CMyEditBox::DeleteCurrentChar()
{
	if( _cursorLocation >= MAX_SIZE )
		return; // no move necessary

    int i = _cursorLocation;
	
	// if there is no character at this position to be deleted
	// just exit.
	if( _string[i] == ' ' )
		return;


	while( _string[i] != '\0' )
	{	
		_string[i] = _string[i+1];
		++i;
	}
	--_size;
}

// moves cursor left one space
// calls DeleteCurrentChar()
void CMyEditBox::DeletePreviusChar()
{
	if( _cursorLocation <= 0 )
		return; // no deletions necessary

	MoveCursorLeft();
	DeleteCurrentChar();
}	

// moves the cursor left one space if possible
void CMyEditBox::MoveCursorLeft()
{

	if( _cursorLocation <=0 )
		return; // no move necessary	
	--_cursorLocation;
}	

// moves the cursor right one space if possible
void CMyEditBox::MoveCursorRight()
{
	// if there is room to move.. then move 
	if( _cursorLocation < MAX_SIZE && _cursorLocation < _size )
		++_cursorLocation;
} 
