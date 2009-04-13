#if !defined CONTROL_H
#define CONTROL_H
//------------------------------------
//  control.h
//  Controller
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "inc/view.h"
#include "inc/timer.h"
#include <windows.h>

class Controller
{
public:
    Controller(HWND hwnd, CREATESTRUCT * pCreate);
    ~Controller ();
	void	Timer (int id);
    void    Size (int x, int y);
    void    Paint ();
    void    Command (int cmd);
    void KeyDown(WPARAM key);

private:

    HWND        _hwnd;
	WinTimer	_timer;
    View        _view;
};

#endif