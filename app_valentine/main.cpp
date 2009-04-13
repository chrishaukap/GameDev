//------------------------------------
//  main.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "inc/main.h"
#include "inc/resource.h"
#include "inc/winmaker.h"
#include "inc/winex.h"
#include <new.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void ChangeToFullScreen(int width, int height)
{		
	DEVMODE dmSettings;				
	memset(&dmSettings,0,sizeof(dmSettings));

	// Get current settings
   if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		MessageBox( NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}    
   dmSettings.dmPelsWidth	= width;			
   dmSettings.dmPelsHeight	= height;	
   dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	// changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

int WINAPI WinMain
    (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
    _set_new_handler (&NewHandler);

    try
    {
        // Create top window class
        FullScreenClass topWinClass ("ValentinesDayClass", hInst, MainWndProc);
        // Is there a running instance of this program?
        HWND hwndOther = topWinClass.GetRunningWindow ();
        if (hwndOther != 0)
        {
            ::SetForegroundWindow (hwndOther);
            if (::IsIconic (hwndOther))
                ::ShowWindow (hwndOther, SW_RESTORE);
            return 0;
        }
        topWinClass.Register ();

        // Create top window
        ResString caption (hInst, ID_CAPTION);
        FullScreenMaker fsWin (topWinClass, caption);
		  fsWin.SetSize (SCREEN_WIDTH, SCREEN_HEIGHT);
        fsWin.Create ();

        ChangeToFullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);	

        fsWin.Show (cmdShow);
        // The main message loop
        MSG  msg;
        int status;
        while ((status = ::GetMessage (&msg, 0, 0, 0)) != 0)
        {
            if (status == -1)
                return -1;
            ::TranslateMessage (&msg);
            ::DispatchMessage (&msg);
        }

        return msg.wParam;
    }
    catch (WinException e)
    {
        char buf [50];
        wsprintf (buf, "%s, Error %d", e.GetMessage (), e.GetError ());
        ::MessageBox (0, buf, "Exception", MB_ICONEXCLAMATION | MB_OK);
    }
    catch (...)
    {
        ::MessageBox (0, "Unknown", "Exception", MB_ICONEXCLAMATION | MB_OK);
    }

    return 0;
}            

int NewHandler (size_t size)
{
    throw WinException ("Out of memory");
    return 0;
}

