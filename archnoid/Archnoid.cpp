// Archnoid.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Archnoid.h"
#include "Ball.h"
#include "GameArea.h"
#include "defs.h"
#include "defaults.h"
#include "BlockCollection.h"
#include <assert.h>

#define MAX_LOADSTRING 100

using namespace Archnoid;



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
GameArea g_gameArea;
Balls g_balls;

BlockCollection g_blockCollection;

HWND g_hwnd;
HDC g_hdcBuffer;
HBITMAP g_hbmBuffer;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void Game_Main();
void Game_Init();
void Game_End();

// Handy Dandy Macros
#define VERTICAL_COLLISION(a) (a == Eastern || a == Western) ? true : false
#define HORIZONTAL_COLLISION(a) (a == Northern || a == Southern) ? true : false


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDC_ARCHNOID, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARCHNOID));

   Game_Init();
	while(1)
   {
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit
			if (msg.message == WM_QUIT)
			break;
		   TranslateMessage(&msg);
		   DispatchMessage(&msg);
      }
      Game_Main();
	}
   Game_End();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARCHNOID));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ARCHNOID);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, 
                       NULL, 
                       WS_MAXIMIZE | WS_POPUP,
                       CW_USEDEFAULT, 
                       0, 
                       CW_USEDEFAULT, 
                       0, 
                       NULL, 
                       NULL, 
                       hInstance, NULL);

   // save the hwnd in a global variable
   g_hwnd = hWnd;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
   case WM_CREATE:
      break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		break;
   case WM_KEYDOWN:
      switch(wParam)
      {
      case VK_ESCAPE:
         PostQuitMessage(0);
         break;
      case VK_SPACE:
         break;
      default:
         PostQuitMessage(0);
         break;
      }
      break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
      
      // TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Game_Main()
{
   HDC hdc = GetDC(g_hwnd);
   RECT rect;
   Ball* ball = NULL;
   static Archnoid::USHORT birthRate = 0;

   // white out the playing bounds
   GetClientRect(g_hwnd,&rect);
   FillRect(g_hdcBuffer,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
   
   // draw the playing bounds
   SelectObject( g_hdcBuffer, GetStockObject( WHITE_BRUSH ) );
   Rectangle(g_hdcBuffer, g_gameArea.GetWestWall(), g_gameArea.GetNorthWall(), 
                          g_gameArea.GetEastWall(), g_gameArea.GetSouthWall());

   //g_blockCollection.RandomMoveAll();
   //g_blockCollection.RandomGravitateToCenterAll();
   // HAUKAP - which ball to gravitate to?
   g_blockCollection.RandomGravitateToPointAll( g_balls.GetFirstBall()->GetCntrX(), g_balls.GetFirstBall()->GetCntrY() );

   if( ++birthRate >= GamePlayDefaults::RESPAWN_RATE )
   {
      birthRate = 0;
      g_blockCollection.NewGenerations();
   }
   
   ball = g_balls.GetFirstBall();
   while( ball )
   {
      ball->Move();

      // did ball go out of bounds?
      CollisionType collision = g_gameArea.OutOfBounds(*ball);
      if( !collision )
      {
         // did ball collide with one of the blocks?
         collision = g_blockCollection.IsCollision(*ball);
      }

      if( collision )
      {
         ball->UnMove();
         if( VERTICAL_COLLISION(collision) )
            ball->ResolveCollision(Vertical);
         else if( HORIZONTAL_COLLISION(collision) )
            ball->ResolveCollision(Horizontal);
      }

      ball = g_balls.GetNextBall();
   }


      
   // draw the blocks   
   SelectObject( g_hdcBuffer, GetStockObject( LTGRAY_BRUSH ) );
   const Block* block = g_blockCollection.GetFirstBlock();
   while( block )
   {
      switch(BlockDefaults::SHAPE)
      {
      case ShapeCircle: 
         Ellipse(g_hdcBuffer, block->GetX(), block->GetY(),
                              block->GetX() + block->GetWidth(),
                              block->GetY() + block->GetHeight());
         break;
      case ShapeRect: 
         Rectangle(g_hdcBuffer,block->GetX(),block->GetY(),
                               block->GetX()+block->GetWidth(),
                               block->GetY()+block->GetHeight());
         break;
      default: assert(!"invalid shape");
      }
      block = g_blockCollection.GetNextBlock();
   }

   // draw the ball
   SelectObject( g_hdcBuffer, GetStockObject( BLACK_BRUSH ) );
   ball = g_balls.GetFirstBall();
   while( ball )
   {
      Ellipse( g_hdcBuffer, ball->GetULX(), ball->GetULY(),
                            ball->GetLRX(), ball->GetLRY());
      ball = g_balls.GetNextBall();
   }

   // draw debug/info text
   const char escMsg[] = "Hit any key to exit";
   TextOut(g_hdcBuffer,5,5,escMsg, (int)strlen(escMsg));


   char buf[256];
   std::string buf1( "Blocks: " );
   buf1 += _itoa_s( g_blockCollection.GetNumBlocks(), buf, 10);
   TextOut(g_hdcBuffer,5,20,buf1.c_str(), (int)buf1.size());

   Archnoid::USHORT y = 40;
   Archnoid::USHORT i = 1;
   ball = g_balls.GetFirstBall();
   while( ball )
   {
      sprintf_s( buf, "Ball[%d] [x:%d y:%d sp:%f vX:%f vY:%f]", i, 
               ball->GetCntrX(), ball->GetCntrY(),
               ball->GetSpeed(), ball->GetVectX(), ball->GetVectY() );
      TextOut(g_hdcBuffer, 5, y, buf, (int)strlen(buf) );

      ball = g_balls.GetNextBall();
      y += 15;
      ++i;
   }

   // copy the game area over to the main hdc
   BitBlt(hdc,0,0,g_gameArea.GetEastWall(),g_gameArea.GetSouthWall(),
          g_hdcBuffer,0,0,SRCCOPY);    

   ReleaseDC(g_hwnd,hdc);
   Sleep(GamePlayDefaults::SLEEPTIME);
}
void Game_Init()
{  
   g_hdcBuffer = CreateCompatibleDC(0);
   g_hbmBuffer = CreateCompatibleBitmap(g_hdcBuffer, 
                                        g_gameArea.GetEastWall(), 
                                        g_gameArea.GetSouthWall());
   SelectObject(g_hdcBuffer, g_hbmBuffer);
   if(!g_hbmBuffer){
      MessageBox(g_hwnd, "Buffer creation failed.", "Error", MB_OK | MB_ICONEXCLAMATION);
      exit(-1);
   }

   
   g_balls.TakeBall(new Ball(BallDefaults::RADIUS, 
                             BallDefaults::X, BallDefaults::Y, 
                             BallDefaults::SPEED, 
                             BallDefaults::VECTOR_X, BallDefaults::VECTOR_Y ));
   g_balls.TakeBall(new Ball(BallDefaults::RADIUS,
                             BallDefaults::X, BallDefaults::Y, 
                             BallDefaults::SPEED,
                             -1* BallDefaults::VECTOR_X, -1* BallDefaults::VECTOR_Y));

   // fill background   
   RECT rect;
   GetClientRect(g_hwnd, &rect);
   FillRect(g_hdcBuffer,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
}
void Game_End()
{
   DeleteDC(g_hdcBuffer);
}