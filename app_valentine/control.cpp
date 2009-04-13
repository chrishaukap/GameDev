//------------------------------------
//  control.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "inc/control.h"
#include "inc/canvas.h"
#include "inc/main.h"
#include "inc/resource.h"
#include "inc/winex.h"
#include "inc/winmaker.h"
#include <string>

// Window Procedure

LRESULT CALLBACK MainWndProc
    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Pointer to Controller is stored in Window
    Controller * pCtrl = WinGetLong<Controller *> (hwnd);

    switch (message)
    {
    case WM_CREATE:
        try
        {
            pCtrl = new Controller (hwnd, reinterpret_cast<CREATESTRUCT *> (lParam));
            // Store pointer to Controller inside Window
            WinSetLong<Controller *> (hwnd, pCtrl);
        }
        catch (WinException e)
        {
			std::string msg (e.GetMessage ());
			char const * obj = e.GetObject ();
			if (obj)
			{
				msg += "\n";
				msg += obj;
			}
            ::MessageBox (hwnd, msg.c_str (), "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            ::MessageBox (hwnd, "Unknown Error", "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
	case WM_TIMER:
		pCtrl->Timer (wParam);
		return 0;
    case WM_SIZE:
        pCtrl->Size (LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_PAINT:
        pCtrl->Paint ();
        return 0;
    case WM_KEYDOWN:
       pCtrl->KeyDown(wParam);
       return 0;
    case WM_COMMAND:
        pCtrl->Command (LOWORD (wParam));
        return 0;
    case WM_DESTROY:
        WinSetLong<Controller *> (hwnd, 0);
        delete pCtrl;
        return 0;
    }
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}

Controller::Controller (HWND hwnd, CREATESTRUCT * pCreate)
   :_hwnd (hwnd), _timer (hwnd, 1), _view (pCreate->hInstance)
{
	_timer.Set (100);
}

Controller::~Controller()
{
    ::PostQuitMessage(0);
}

void Controller::Timer (int id)
{
	_timer.Kill ();
	_view.Step ();
	UpdateCanvas canvas (_hwnd);
	_view.Update (canvas);
	::InvalidateRect (_hwnd, 0, false);
	_timer.Set (8);
}

void Controller::Size (int cx, int cy)
{
    _view.SetSize (cx, cy);
}

void Controller::Paint ()
{
    PaintCanvas canvas (_hwnd);
    _view.Paint (canvas);
}

// key down event processing
void Controller::KeyDown(WPARAM key)
{							
   switch (key) 	
   {		
      case VK_ESCAPE:	
         PostQuitMessage(0);
         break;
      case VK_LEFT:
         _view.heart()->moveLeft();
         break;
      case VK_UP:	
         _view.heart()->moveUp();
         break;
      case VK_RIGHT:	
         _view.heart()->moveRight();
         break;
      case VK_DOWN:	
         _view.heart()->moveDown();
         break;
   }
}

// Menu commands processing

void Controller::Command (int cmd)
{
    switch (cmd)
    {
    case IDM_EXIT:
        ::SendMessage (_hwnd, WM_CLOSE, 0, 0L);
        break;
    case IDM_HELP:
        ::MessageBox (_hwnd, "Go figure!",
            "Bitmap", MB_ICONINFORMATION | MB_OK);
        break;
    case IDM_ABOUT:
        {
            // Instance handle is available through HWND
            HINSTANCE hInst = WinGetLong<HINSTANCE> (_hwnd, GWL_HINSTANCE);
            ::DialogBox (hInst, 
                         MAKEINTRESOURCE (IDD_ABOUT), 
                         _hwnd, 
                         AboutDlgProc);
        }
        break;
    }
}

// "About" dialog box procedure
// Process messages from dialog box
// Caution: use Windows BOOL, not C++ bool

BOOL CALLBACK AboutDlgProc
   (HWND hwnd, UINT message, UINT wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case IDOK:
        case IDCANCEL:
            ::EndDialog (hwnd, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

