#if !defined (TIMER_H)
#define TIMER_H
//-------------------------------------
// Copyright Reliable Software (c) 1999
//-------------------------------------

#include <windows.h>

class WinTimer
{
public:
	WinTimer (HWND hwnd = 0, int id = -1) 
		: _hwnd (hwnd), _id (id)
	{}

	void Create (HWND hwnd, int id)
	{
		_hwnd = hwnd;
		_id = id;
	}

	void Set (int milliSec)
	{
		::SetTimer (_hwnd, _id, milliSec, 0);
	}

	void Kill ()
	{
		::KillTimer (_hwnd, _id);
	}
	int  GetId () const { return _id; }
private:
	HWND	_hwnd;
	int		_id;
};

#endif