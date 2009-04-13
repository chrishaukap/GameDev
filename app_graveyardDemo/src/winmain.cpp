#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <assert.h>

#include "time.h"
#include "CGfxOpenGL.h"
#include "GameEngine.h"
#include "defines.h"
#include "camera.h"
#include "vector3d.h"
#include "peeps/peep.h"

bool exiting = false;
long windowWidth = SCREEN_WIDTH;
long windowHeight = SCREEN_HEIGHT;
long windowBits = 32;
bool fullscreen = false;
bool g_paused = BEGIN_PAUSED;
HDC hDC;

class GameStateManager
{
private:
   enum GameState
   {
      StateInit,
      StateMenu,
      StateRunning,
      StateCredits,
      StateInvalid
   };
public:
   GameStateManager(CGfxOpenGL& renderer, GameEngine& engine) :
      m_state(StateInit),
      m_renderer(renderer),
      m_engine(engine)
   {}   

   void initialize()
   {
      switch(m_state)
      {
      case StateInit: 
         m_renderer.Init();
         m_engine.initializePreCredits();
         menu();
         break;
      case StateMenu: 
      case StateRunning: 
      case StateCredits: 
      case StateInvalid:
      default:
         assert(!"INVALID STATE");
         break;      
      }
   }
   void process()
   {
      switch(m_state)
      {
      case StateInit: assert(!"NOT INITIALIZED"); break;
      case StateMenu: 
         break;
      case StateRunning: 
         m_engine.process(); 
         break;
      case StateCredits: 
         break;
      case StateInvalid:
      default:
         assert(!"INVALID STATE");
         break;
      }
   }
   void render()
   {
      switch(m_state)
      {
      case StateInit: assert(!"NOT INITIALIZED"); break;
      case StateMenu: 
         m_renderer.renderMenu();
         break;
      case StateRunning:
         m_renderer.render();
         break;
      case StateCredits:
         m_renderer.renderCreditScreen();
         break;
      case StateInvalid:
      default:
         assert(!"INVALID STATE");
         break;
      }
   }
   void menu() {m_state = StateMenu;}
   void run() 
   {
      static CHUint timesCalled = 0;
      assert( ++timesCalled == 1 );

      m_engine.initializePostCredits();
      m_state = StateRunning;
   }
   void credits() {m_state = StateCredits;}
   bool inMenu() const {return m_state == StateMenu;}
   bool running() const {return m_state == StateRunning;}
   bool inCredits() const {return m_state == StateCredits;}

private:   
   GameState m_state;
   CGfxOpenGL &m_renderer;
   GameEngine &m_engine;
};
GameStateManager *g_gameStateMngr;
CGfxOpenGL *g_glRender = NULL;
GameEngine *g_gameEngine = NULL;

void SetupPixelFormat(HDC hDC)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER,           // support double-buffering
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, CHUint uMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hDC;
    static HGLRC hRC;
    int height, width;
    static bool mousedown = false;
    static POINT prevPoint;
    static bool close = false;

    // dispatch messages
    switch (uMsg)
    {   
    case WM_CREATE:         // window creation
        hDC = GetDC(hWnd);
        SetupPixelFormat(hDC);
        //SetupPalette();
        hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hRC);

        srand((CHUint)time(NULL));   
        break;

    case WM_DESTROY:            // window destroy
    case WM_QUIT:
    case WM_CLOSE:                  // windows is closing

        // deselect rendering context and delete it
        wglMakeCurrent(hDC, NULL);
        wglDeleteContext(hRC);

        // send WM_QUIT to message queue
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        height = HIWORD(lParam);        // retrieve width and height
        width = LOWORD(lParam);

        g_glRender->SetupProjection(width, height);

        break;

    case WM_ACTIVATEAPP:        // activate app
        break;

    case WM_PAINT:              // paint
      {
         PAINTSTRUCT ps;
         BeginPaint(hWnd, &ps);
         EndPaint(hWnd, &ps);
      }
      break;

    case WM_LBUTTONDOWN:        // left mouse button
      {
         prevPoint.x = LOWORD(lParam);
         prevPoint.y = HIWORD(lParam);
         mousedown = true;
      }
      break;
    case WM_LBUTTONUP: // left mouse up
       mousedown = false;
       break;

    case WM_RBUTTONDOWN:        // right mouse button
        break;
    case WM_RBUTTONUP:          // right button release
        break;

    case WM_MOUSEMOVE:          // mouse movement
       if(mousedown)
       {          
          POINT curPoint;
          GetCursorPos(&curPoint);
          ScreenToClient(hWnd, &curPoint);
          int dx = curPoint.x - prevPoint.x;
          int dy = curPoint.y - prevPoint.y;
          float multx = 0.1f, multy = 0.5f;
          
          prevPoint.x = curPoint.x;
          prevPoint.y = curPoint.y;
       }
       break;


    case WM_KEYUP:
        break;

   case WM_KEYDOWN:
      int fwKeys;
      LPARAM keyData;
      fwKeys = (int)wParam;    // virtual-key code 
      keyData = lParam;          // key data 

      switch(fwKeys)
      {
         case VK_ESCAPE:
            if(close || g_gameStateMngr->inMenu())
               PostQuitMessage(0);
            if(!close)
            {
               g_gameStateMngr->credits();
               close = true;
            }
            break;
         case VK_RETURN:
            if(g_gameStateMngr->inMenu())
               g_gameStateMngr->run();
            if(g_paused)
               g_gameStateMngr->process();
            break;
         case 'P':
            g_paused = !g_paused;
            break;
         case 'Z':
            g_gameEngine->camera()->zoomIn();
            break;
         case 'X':
            g_gameEngine->camera()->zoomOut();
            break;
         case VK_LEFT:
            g_gameEngine->camera()->moveLateralDistance( -10 );
            break;
         case VK_RIGHT:
            g_gameEngine->camera()->moveLateralDistance( 10 );
            break;
         case VK_DOWN:
            g_gameEngine->camera()->down();
            break;
         case VK_UP:
            g_gameEngine->camera()->up();
            break;
         default:
            break;
      }

      break;

   default:
      break;
   }
   return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX windowClass;     // window class
    HWND       hwnd;            // window handle
    MSG        msg;             // message
    DWORD      dwExStyle;       // Window Extended Style
    DWORD      dwStyle;         // Window Style
    RECT       windowRect;

    g_gameEngine = new GameEngine;
    g_glRender = new CGfxOpenGL(*g_gameEngine);
    g_gameStateMngr = new GameStateManager(*g_glRender, *g_gameEngine);

    windowRect.left=(long)0;                        // Set Left Value To 0
    windowRect.right=(long)windowWidth; // Set Right Value To Requested Width
    windowRect.top=(long)0;                         // Set Top Value To 0
    windowRect.bottom=(long)windowHeight;   // Set Bottom Value To Requested Height

    // fill out the window class structure
    windowClass.cbSize          = sizeof(WNDCLASSEX);
    windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc     = MainWindowProc;
    windowClass.cbClsExtra      = 0;
    windowClass.cbWndExtra      = 0;
    windowClass.hInstance       = hInstance;
    windowClass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);  // default icon
    windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);      // default arrow
    windowClass.hbrBackground   = NULL;                             // don't need background
    windowClass.lpszMenuName    = NULL;                             // no menu
    windowClass.lpszClassName   = "GLClass";
    windowClass.hIconSm         = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon

    // register the windows class
    if (!RegisterClassEx(&windowClass))
        return 0;

    if (fullscreen)                             // fullscreen?
    {
        DEVMODE dmScreenSettings;                   // device mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings); 
        dmScreenSettings.dmPelsWidth = windowWidth;         // screen width
        dmScreenSettings.dmPelsHeight = windowHeight;           // screen height
        dmScreenSettings.dmBitsPerPel = windowBits;             // bits per pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // 
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // setting display mode failed, switch to windowed
            MessageBox(NULL, "Display mode failed", NULL, MB_OK);
            fullscreen = FALSE; 
        }
    }

    if (fullscreen)                             // Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW;                  // Window Extended Style
        dwStyle=WS_POPUP;                       // Windows Style
        ShowCursor(FALSE);                      // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW;                    // Windows Style
    }

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

    // class registered, so now create our window
    hwnd = CreateWindowEx(NULL,                                 // extended style
        "GLClass",                          // class name
        "BOGLGP - Chapter 2 - OpenGL Application",      // app name
        dwStyle | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS,
        SCREEN_X,SCREEN_Y,                               // x,y coordinate
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, // width, height
        NULL,                               // handle to parent
        NULL,                               // handle to menu
        hInstance,                          // application instance
        NULL);                              // no extra params

   hDC = GetDC(hwnd);

   // check if window creation failed (hwnd would equal NULL)
   if (!hwnd)
     return 0;

   ShowWindow(hwnd, SW_SHOW);          // display the window
   UpdateWindow(hwnd);                 // update the window

   g_gameStateMngr->initialize();

   while (!exiting)
   {
      if( !g_paused )
         g_gameStateMngr->process();
      g_gameStateMngr->render();

      Sleep(SLEEP_TIME);
      SwapBuffers(hDC);

      while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
      {
         if (!GetMessage (&msg, NULL, 0, 0))
         {
            exiting = true;
            break;
         }

         TranslateMessage (&msg);
         DispatchMessage (&msg);
      }
   }

   delete g_gameStateMngr;
   delete g_gameEngine; 
   delete g_glRender;

    if (fullscreen)
    {
        ChangeDisplaySettings(NULL,0);          // If So Switch Back To The Desktop
        ShowCursor(TRUE);                       // Show Mouse Pointer
    }

    return (int)msg.wParam;
}