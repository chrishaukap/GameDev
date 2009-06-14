
#define INITGUID       // make sure all the COM interfaces are available
                       // instead of this you can include the .LIB file
                       // DXGUID.LIB

#define WIN32_LEAN_AND_MEAN  

#include "T3DLIB1.h" // game library includes
#include "T3DLIB2.h"
#include "T3DLIB3.h"
#include "block.h"
#include "blockcollection.h"
#include "globals.h"
#include "title.h"
#include "highscores.h"
#include "MyEditBox.h"
#include "CFGManager.h"
#include "Logger.h"

#include "dsutil.h"
#include "dxutil.h"








const int SHIFT_TIME = 20;
const int ROTATION_TIME = 4;
const int SIDE_TIME = 2;
const int CASCADE_TIME = 30;
const int DOWN_TIME = 1;
const int QUIT_TIMER = 10;
const int ENTER_TIMER = 10;
const int STATE_NORMAL = 0;
const int STATE_SHIFT = 1;
const int STATE_CASCADE = 2;
const int STATE_GAME = 0;
const int STATE_MENU = 1;
const int STATE_CRED = 2;
char BACKGROUND_BITMAP_STR[]= "img\\tetrisback_24.bmp";
char POPUP_BITMAP_STR[] = "img\\popup_24.bmp";
char SQUARE_BITMAP_STR[] = "img\\block_24.bmp";
char START_BUTTON_OUT_STR[] ="img\\start_button_out_24.bmp";
char START_BUTTON_OVER_STR[] ="img\\start_button_over_24.bmp";
char MENU_BG_STR[] = "img\\menu_background_24.bmp";
char POINTER_STR[] = "img\\pointer_24.bmp";
char BACKGROUND_MENU_MUSIC[] = "sounds\\sp066.wav";
char BACKGROUND_GAME_MUSIC[] = "sounds\\techno.wav";

// this extracts the RGB components of a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB565FROM16BIT(RGB, r,g,b) { *r = ( ((RGB) >> 11) & 0x1f); *g = (((RGB) >> 5) & 0x3f); *b = ((RGB) & 0x1f); }
// how the blocks type and color are chosen
#define TYPE_DETERMINATION rand()%7
//#define TYPE_DETERMINATION 0

// GLOBALS ////////////////////////////////////////////////
HWND main_window_handle           = NULL; // save the window handle
HINSTANCE main_instance           = NULL; // save the instance
char buffer[80];                          // used to print text
bool menu_up = true;
HANDLE thread_handle;
DWORD  thread_id;
int game_state = STATE_MENU;

Logger g_logger;

//  ALPHA BLENDING ITEMS/////////////////////////////////
//
//
//	BITMAP_IMAGE textures1,  // holds source texture library  1
//				 textures2,  // holds source texture library  2
	BITMAP_IMAGE alpha_dest;            // temporary working texture

//  MENU ITEMS ///////////////////////////////////////////
//
//
	CTitle title;

	BOB Pointer;						// mouse pointer bob
	BITMAP_IMAGE start_button_out_bmp;	// mouse-out start button bmp
	BITMAP_IMAGE start_button_over_bmp;	// mouse-over start button bmp
	BITMAP_IMAGE menu_bg_bmp;			// menu background bmp
		// clickable regions for menu items
	RECT button_start_region = {230,400,415,445};

// GAME PLAY ITEMS ///////////////////////////////////////
//
//
			// scoring
	CHighScores high_scores;
	int g_score = 0;						// player score
	int g_speed = SPEED[0];				// speed of falling block
	int g_level = MIN_LEVEL;						// current level of difficulty
	char player_name[20] = "123456789012345\0";// players name
	int current_state = STATE_NORMAL;	// current state of the game
   CFGManager* g_cfgManager = NULL;
   uint16 g_windowed = WINDOWED;

// GAME GRAPHIC OBJECTS //////////////////////////////////
//
//
	BITMAP_IMAGE background_bmp;	// background
	BITMAP_IMAGE popup_bmp;			// the popup dialog box for user name gathering
	Block* g_block = NULL;                             // the falling block		
	Block* g_previewBlock = NULL;                      // preview block
	BlockCollection* g_blockCollection  = NULL;        // the collection of blocks	
	BOB BlockColors[MAX_COLORS];	// 1 BOB object for each color of block
	int anim_reg[] = {0,1,2,3,4,5,6,7,8,9};
	int anim_trans[] = {10,11,12,13,14,15};
	int anim_flash[] = {16,17,18,19,20,21,22,23,24,25};

// MUSIC ITEMS ///////////////////////////////////////////
//
//
   namespace SoundIDs
   {
      enum SoundID
      {
         SND_GAME_ID,
         SND_MENU_ID,
         SND_LEVEL_ID,
         SND_GAMEOVER_ID,
         SND_BLOCKROT_ID,
         SND_BLOCKADD_ID
      };
   }
   CSound* g_sounds[10] = {NULL};	// array of gameplay sounds
   CSoundManager* g_soundmgr = NULL;


// TIMING COUNTERS //////////////////////////////////////
//
//
	int rotation_counter = ROTATION_TIME;
	int side_counter = SIDE_TIME;
	int down_counter = DOWN_TIME;
	int cascade_counter = 0;
	int shift_counter = 0;
	int quit_counter = 0;
	int enter_counter = 0;

// PROTOTYPES /////////////////////////////////////////////
//
//

	// alpha blending function
	void MyAlphaBlend(USHORT *tbuffer, BITMAP_IMAGE* alpha_dest, USHORT *s1buffer, 
		USHORT *s2buffer, float alphaf);

	// thread used to collect to create a popup that collects users name
	DWORD WINAPI Retrieve_Name_Thread(LPVOID data);
	
	// prints an integer to screen using DirectDraw
	void printInt( int x, int y, int valToPrint, COLORREF color );
	
	// resets the game when the user loses
	void ResetGame();

	// loads the preview block into the current block, and 
	// loads a new random preview block
	void LoadBlock();

	// reads in the highscores from a file and stores
	// those scores in the high_scores class
	void ReadHighScores();

	// writes the highschores from the high_scores class to a file
	void WriteHighScores();

	// loads all sounds used by the game
	int LoadSounds();

	// determines if the DirectDraw mouse is in a given region (rect)
	bool MouseInRegion(int mouse_x, int mouse_y, RECT& rect);

	
	void ButtonActivate( char button, int* pressed, char* character, bool* button_pressed);

	// GAME STATE FUNCTIONS
	void DoStateNormal(int& no_input);
	void DoStateShift(int& no_input);
	void DoStateCascade();

	// game console functions
	int Game_Init(void *parms=NULL);
	int Game_Shutdown(void *parms=NULL);
	int Game_Main(void *parms=NULL);
	int Menu_Main(void *parms=NULL);
	int Credits_Main();


// FUNCTIONS //////////////////////////////////////////////
// WINPROC ////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   // handle to a device context

	// what is the message 
	switch(msg)
		{	
		case WM_CREATE: 
			{
				// do initialization stuff here
				
				// select a monospaced font 
				//	main_instance = ((LPCREATESTRUCT) lparam)->hInstance;
				return(0);
			} break;

		//case(msg) WM_P_GRAPHNOTIFY:
		//	{
		//		HandleEvent();
		//		break;
		//	}
		case WM_PAINT:
			{
			// start painting
			hdc = BeginPaint(hwnd,&ps);

			// end painting
			EndPaint(hwnd,&ps);
			return(0);
			} break;

		case WM_DESTROY: 
			{
			// kill the application			
			PostQuitMessage(0);
			return(0);
			} break;

		default:break;

		} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{
	// this is the winmain function

	WNDCLASSEX winclass; // this will hold the class we create
	HWND	   hwnd;	 // generic window handle
	MSG		   msg;		 // generic message
	HDC        hdc;      // graphics device context

	// first fill in the window class stucture
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
							CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	// save hinstance in global
	main_instance = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		return(0);

   
   g_logger.open(g_logLevel, g_logFile);   
   g_cfgManager = new CFGManager(g_logger);
   MyStatus sts = g_cfgManager->init("cfg.txt");
   sts = g_cfgManager->parse();
   g_windowed = g_cfgManager->getFullScreen();
#ifdef _DEBUG
   if(FORCE_WINDOWED)
      g_windowed = WINDOWED;
#endif

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,                  // extended style
								WINDOW_CLASS_NAME,     // class
								WINDOW_TITLE, // title
								(g_windowed ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)), 
					 			0,0,	  // initial x,y
								WINDOW_WIDTH,WINDOW_HEIGHT,  // initial width, height
								NULL,	  // handle to parent 
								NULL,	  // handle to menu
								hinstance,// instance of this application
								NULL)))	// extra creation parms
	return(0);

	// save main window handle
	main_window_handle = hwnd;

	if (g_windowed)
	{
		// now resize the window, so the client area is the actual size requested
		// since there may be borders and controls if this is going to be a windowed app
		// if the app is not windowed then it won't matter
		RECT window_rect = {0,0,WINDOW_WIDTH-1,WINDOW_HEIGHT-1};

		// make the call to adjust window_rect
		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(main_window_handle),
			GetMenu(main_window_handle) != NULL,
			GetWindowExStyle(main_window_handle));

		// save the global client offsets, they are needed in DDraw_Flip()
		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;

		// now resize the window with a call to MoveWindow()
		MoveWindow(main_window_handle,
				0, // x position
				0, // y position
				window_rect.right - window_rect.left, // width
				window_rect.bottom - window_rect.top, // height
				TRUE);

		// show the window, so there's no garbage on first render
		ShowWindow(main_window_handle, SW_SHOW);
	} // end if windowed



	// perform all game console specific initialization
	Game_Init();

	// enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit
			if (msg.message == WM_QUIT)
			break;
		
			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
	    
		// main game processing goes here
		switch( game_state )
		{
		   case STATE_GAME: Game_Main(); break;
		   case STATE_MENU: Menu_Main(); break;
		   case STATE_CRED: Credits_Main(); break;
		}

	} // end while

	// shutdown game and release all resources
	Game_Shutdown();

	// return to Windows like this
	return(int)(msg.wParam);

} // end WinMain

// T3D GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////


int Game_Init(void *parms)
{		
   g_logger.logDebug("Game_Init()");

   g_level = g_cfgManager->getStartingLevel();
   g_score = 0;

   g_block = new Block(g_logger);
   g_previewBlock = new Block(g_logger);
   g_blockCollection = new BlockCollection(g_logger);

	// get rid of the windows mouse cursor
	ShowCursor(false);

	// start up DirectInput
	if(!DInput_Init()) return 0;

	// initialize the keyboard
	if(!DInput_Init_Keyboard()) return 0;
	if(!DInput_Init_Mouse()) return 0;
	
	// seed the random number generator
	srand( (unsigned)time( NULL ) );

	// initialize directdraw, very important that in the call
	// to setcooperativelevel that the flag DDSCL_MULTITHREADED is used
	// which increases the response of directX graphics to
	// take the global critical section more frequently
	if(!DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, g_windowed))
   {
      g_logger.logError("DDraw_Init() Failed");
      return 0;
   }

	// TODO: image information should be loaded into a struct, and that struct should be passed to a 
	// image-loading function which will load file, create bitmap, load image, and unload the file.

	// load background image
	if(!Load_Bitmap_File(&bitmap16bit, BACKGROUND_BITMAP_STR))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }
	if(!Create_Bitmap(&background_bmp,0,0,640,480,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }
	if(!Load_Image_Bitmap16(&background_bmp, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }
	if(!Unload_Bitmap_File(&bitmap16bit))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }
	
	// load the popup bg image
	if(!Load_Bitmap_File(&bitmap16bit, POPUP_BITMAP_STR))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }
	if(!Create_Bitmap(&popup_bmp,140,110,367,221,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }
	if(!Load_Image_Bitmap16(&popup_bmp, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }
	if(!Unload_Bitmap_File(&bitmap16bit))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }

	// load menu images
	if(!Load_Bitmap_File( &bitmap16bit, START_BUTTON_OVER_STR ))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }

	if(!Create_Bitmap( &start_button_over_bmp,0,370,640,110,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }

	if(!Load_Image_Bitmap16( &start_button_over_bmp, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }

	if(!Unload_Bitmap_File( &bitmap16bit ))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }


	if(!Load_Bitmap_File( &bitmap16bit, START_BUTTON_OUT_STR ))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }

	if(!Create_Bitmap( &start_button_out_bmp,0,370,640,110,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }

	if(!Load_Image_Bitmap16( &start_button_out_bmp, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }

	if(!Unload_Bitmap_File( &bitmap16bit ))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }

	if(!Load_Bitmap_File( &bitmap16bit, MENU_BG_STR ))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }

	if(!Create_Bitmap( &menu_bg_bmp,0,0,640,480,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }

	if(!Load_Image_Bitmap16( &menu_bg_bmp, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }

	if(!Unload_Bitmap_File( &bitmap16bit ))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }


	///////////////////////
	// ALPHA BLENDING TEST
	///////////////////////
	if(!Load_Bitmap_File( &bitmap16bit, START_BUTTON_OVER_STR ))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }

	if(!Create_Bitmap( &alpha_dest,0,370,640,110,16))
   {
      g_logger.logError("Create_Bitmap() Failed");
      return 0;
   }

	if(!Load_Image_Bitmap16( &alpha_dest, &bitmap16bit,0,0,BITMAP_EXTRACT_MODE_ABS))
   {
      g_logger.logError("Load_Image_Bitmap16() Failed");
      return 0;
   }

	if(!Unload_Bitmap_File( &bitmap16bit ))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }


	// load the bitmaps for the various colors of blocks

	if(!Load_Bitmap_File(&bitmap16bit, SQUARE_BITMAP_STR))
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }


	for( int i=0; i<MAX_COLORS; ++i )
	{
		if(!Create_BOB( &(BlockColors[i]),
                      START_X, 
                      START_Y,
                      SQUARE_WIDTH,
                      SQUARE_WIDTH,
                      26,
                      BOB_ATTR_MULTI_ANIM | BOB_ATTR_VISIBLE, 
                      DDSCAPS_SYSTEMMEMORY,
                      0,
                      16))
      {
         g_logger.logError("Create_BOB() Failed");
         return 0;
      }
	
		// load the frames
		// load the regular frames
		for (int index=0; index<NUM_REG_FRAMES; ++index)
      {
         if(!Load_Frame_BOB16( &(BlockColors[i]), 
                               &bitmap16bit,
                               index, 
                               index,
                               i,
                               BITMAP_EXTRACT_MODE_CELL))
         {
            g_logger.logError("Load_Frame_BOB16() Failed");
            return 0;
         }
      }
		// load the flash frames
		for (int index=0; index<NUM_FLASH_FRAMES; ++index)
      {
         if(!Load_Frame_BOB16( &(BlockColors[i]), 
                               &bitmap16bit,
                               index + NUM_REG_FRAMES,
                               index, 
                               7, 
                               BITMAP_EXTRACT_MODE_CELL))
         {
            g_logger.logError("Load_Frame_BOB16() Failed");
            return 0;
         }
      }
		for (int index=0; index<NUM_TRANS_FRAMES; ++index)
      {
         if(!Load_Frame_BOB16( &(BlockColors[i]), 
                               &bitmap16bit,
                               index + NUM_REG_FRAMES + NUM_FLASH_FRAMES, 
                               index, 
                               ((i<4)?i:3)+8, 
                               BITMAP_EXTRACT_MODE_CELL))
         {
            g_logger.logError("Load_Frame_BOB16() Failed");
            return 0;
         }
      }

		// load animations
		if(!Load_Animation_BOB(&(BlockColors[i]),0,NUM_REG_FRAMES,anim_reg))
      {
         g_logger.logError("Load_Animation_BOB() Failed");
         return 0;
      }
		if(!Load_Animation_BOB(&(BlockColors[i]),2,NUM_TRANS_FRAMES,anim_trans))
      {
         g_logger.logError("Load_Animation_BOB() Failed");
         return 0;
      }
		if(!Load_Animation_BOB(&(BlockColors[i]),1,NUM_FLASH_FRAMES,anim_flash))
      {
         g_logger.logError("Load_Animation_BOB() Failed");
         return 0;
      }

		if(!Set_Animation_BOB(&(BlockColors[i]), 0))
      {
         g_logger.logError("Set_Animation_BOB() Failed");
         return 0;
      }

		// set the animation speed
		if(!Set_Anim_Speed_BOB( &(BlockColors[i]), ANIMATION_SPEED ))
      {
         g_logger.logError("Set_Anim_Speed_BOB() Failed");
         return 0;
      }
	}

	// unload squares bitmap
	if(!Unload_Bitmap_File(&bitmap16bit))
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }

	// load the main block and the falling block
	int tmp=TYPE_DETERMINATION;
	g_block->CreateBlock((BlockType)tmp, &BlockColors[tmp], START_X, START_Y);

	tmp=TYPE_DETERMINATION;
	g_previewBlock->CreateBlock((BlockType)tmp, &BlockColors[tmp], START_PREVIEW_X, START_PREVIEW_Y);

	// create the mouse pointer
	if( !Load_Bitmap_File(&bitmap16bit, POINTER_STR) ) 
   {
      g_logger.logError("Load_Bitmap_File() Failed");
      return 0;
   }
	
	if( !Create_BOB( &Pointer,
                    SCREEN_WIDTH / 2, 
                    SCREEN_HEIGHT / 2,
                    23, 23, 1,
                    BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
                    DDSCAPS_SYSTEMMEMORY, 
                    0, 16) )
   {
      g_logger.logError("Create_BOB() Failed");
      return 0;
   }
	if( !Load_Frame_BOB16(&Pointer,&bitmap16bit,0,0,0,BITMAP_EXTRACT_MODE_CELL) ) 
   {
      g_logger.logError("Load_Frame_BOB16() Failed");
      return 0;
   }

	if( !Unload_Bitmap_File(&bitmap16bit) )
   {
      g_logger.logError("Unload_Bitmap_File() Failed");
      return 0;
   }

	// create the Title
	title.InitTitle( BlockColors );
	
	// load sound effects
	if( !LoadSounds() )
   {
      g_logger.logError("LoadSounds() Failed");
      return 0;
   }

	// read in the high scores	
	ReadHighScores();
	
   if( FAILED(g_sounds[SoundIDs::SND_MENU_ID]->Play(0,DSBPLAY_LOOPING)))
   {
      g_logger.logError("DSound_Play() Failed");
      return 0;
   }

	// return success
	return(1);

} // end Game_Init

///////////////////////////////////////////////////////////



int Game_Shutdown(void *parms)
{
   g_logger.logDebug("Game_Shutdown()");
   delete g_block;
   delete g_previewBlock;
   delete g_blockCollection;
   delete g_cfgManager;
   
	// write high scores before quitting	
	WriteHighScores();
    
	// shutdown directdraw last
	DDraw_Shutdown();
	
	// delete block colors
	
	for( int i=0; i<MAX_COLORS; ++i )
		Destroy_BOB(&(BlockColors[i]));
	Destroy_BOB( &Pointer );
	
	title.DestroyTitle();

	// release keyboard
	DInput_Release_Mouse();
	DInput_Release_Keyboard();	

	// shutdown DirectInput
	DInput_Shutdown();   
   
   int i=0;
   while(g_sounds[i])
   {
      delete(g_sounds[i]);
      ++i;
   }
   delete g_soundmgr;


   g_logger.close();

	// return success
	return(1);
} // end Game_Shutdown



//////////////////////////////////////////////////////////
int Menu_Main(void *parms)
{	
	// take user input for movement and rotations	
	if( !DInput_Read_Keyboard() ) return 0;
	if( !DInput_Read_Mouse() ) return 0;

	// check if user is trying to exit
	if (keyboard_state[DIK_ESCAPE] && ++quit_counter >= QUIT_TIMER )
	{
		quit_counter = 0;
		game_state = STATE_CRED;
		return (1);
	} // end if
	
	// if user presses enter, start the game
	if(keyboard_state[DIK_RETURN] && ++enter_counter >= ENTER_TIMER)
	{
		game_state = STATE_GAME;
      if(FAILED(g_sounds[SoundIDs::SND_MENU_ID]->Stop()))
      {
         g_logger.logError("failed to stop menu sound");
         return 1;
      }      
      if(FAILED(g_sounds[SoundIDs::SND_GAME_ID]->Play(0,DSBPLAY_LOOPING)))
      {
         g_logger.logError("failed to start game music");
         return 1;
      }
	} //end if

	// lock the back buffer for writing
	DDraw_Lock_Back_Surface();  
	// draw the background for the menu
	Draw_Bitmap16(&menu_bg_bmp, back_buffer, back_lpitch,0 );   

	// update the cursor location with users mouse movements
	Pointer.x += mouse_state.lX;
	Pointer.y += mouse_state.lY;

	// check for out of bounds on the mouse cursor
	if( Pointer.x < 0 ) Pointer.x = 0;
	if( Pointer.x > SCREEN_WIDTH ) Pointer.x = SCREEN_WIDTH;
	if( Pointer.y < 0 ) Pointer.y = 0;
	if( Pointer.y > SCREEN_HEIGHT ) Pointer.y = SCREEN_HEIGHT;
  

	///////////////////////////////////////////////////////////////////////////
	//		ALPHA BLENDING TEST
	///////////////////////////////////////////////////////////////////////////

	USHORT *s1buffer = (USHORT *)menu_bg_bmp.buffer;
	USHORT *s2buffer = (USHORT *)start_button_over_bmp.buffer;
	USHORT *tbuffer  = (USHORT *)alpha_dest.buffer;
	static float alphaf = 0.99f;
	static int plus_or_minus = 0;  

	MyAlphaBlend( tbuffer, &alpha_dest, s1buffer, s2buffer, alphaf );

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////




	// if user hovers over the region of the start button, draw the over bitmap
	if( MouseInRegion( (int)Pointer.x, (int)Pointer.y, button_start_region )  )
	{
		//Draw_Bitmap16(&alpha_dest, back_buffer, back_lpitch,0);
		Draw_Bitmap16(&start_button_over_bmp, back_buffer, back_lpitch, 0 );
		if( mouse_state.rgbButtons[0] )
		{
			game_state = STATE_GAME;
			//DSound_Stop_Sound(menuMusicID);
			//DSound_Play(SND_MENU_ID, DSBPLAY_LOOPING);
		}		
	}
	else
		Draw_Bitmap16(&alpha_dest, back_buffer, back_lpitch,0);
	
	// adjust the alpha number
	if( plus_or_minus )
		alphaf += 0.01f;
	else
		alphaf -= 0.01f;
	if( alphaf >= 0.85f )
	{
		alphaf = 0.84f;
		plus_or_minus = 0;
	}
	else if( alphaf <= 0 )
	{
		alphaf = 0.01f;
		plus_or_minus = 1;
	}


	// unlock back buffer
	DDraw_Unlock_Back_Surface();  
	

	// draw the "tetrix!" Title
	title.DrawTitle();
	// draw the cursor on top of everything else
	Draw_BOB16(&Pointer,lpddsback);

	char name[25],tmp[10],blah[3];
	int score;
	int num_scores = high_scores.GetNumScores();

	Draw_Text_GDI( "HIGH SCORES", 270, 170, RGB(255,255,255), lpddsback );
	for( int i=0;i< num_scores; ++i )
	{	
		high_scores.HighScoreAtIndex( i, tmp, score );
		strcpy( name, itoa(i+1,blah,10));
		strcat( name, ": " );
		strcat( name, tmp );
		Draw_Text_GDI(	name, 
						150 + ((i<=4) ? 0 : 210),
						i*25+200 + ((i<=4) ? 0 : -125), 
						RGB(255,255,255), lpddsback );
		printInt(		250 + ((i<=4) ? 0 : 210), 
						i*25+200 + ((i<=4) ? 0 : -125), 
						score, RGB(255,255,255) );
	}

	DDraw_Flip();						// flip the surfaces	
	Wait_Clock(30);						// sync to 30ish fps	
	return(1);							// return success
}
///// CREDITS SCREEN
int Credits_Main()
{
	// take user input for movement and rotations	
	if( !DInput_Read_Keyboard() ) return 0;

	// check if user is trying to exit
	if (keyboard_state[DIK_ESCAPE] && ++quit_counter >= QUIT_TIMER )
	{
		PostMessage(main_window_handle, WM_DESTROY,0,0);
		// stop all sounds
		//DSound_Stop_All_Sounds();
		return (1);
	}

	DDraw_Lock_Back_Surface();  // lock the back buffer for writing
	Draw_Bitmap16(&menu_bg_bmp, back_buffer, back_lpitch,0 );
	DDraw_Unlock_Back_Surface();  // unlock back buffer
	
	Draw_Text_GDI( "Thanks for playing!  I would appreciate any ideas", 100, 100, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "or constructive criticisms you might have.  ", 100, 115, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "Let me know what you think:", 100, 150, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "email:", 100, 165, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "chrishaukap@gmail.com", 150, 165, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "phone:", 100, 180, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "425-344-0838", 150, 180, RGB(255,255,255), lpddsback );
	Draw_Text_GDI( "web:", 100, 195, RGB(255,255,255), lpddsback );
   Draw_Text_GDI( "www.evilmordekai.com", 150, 195, RGB(255,255,255), lpddsback );

	DDraw_Flip();						// flip the surfaces	
	Wait_Clock(30);						// sync to 30ish fps	
	return(1);							// return success
}
//////////////////////////////////////////////////////////
int Game_Main(void *parms)
{
	int index=0;
	static int num_adds = 0;
	static int num_cascades=0;
	static bool paused = false;
	static bool cascading = false;
	static int no_input = 0;
	bool block_added = false;
	bool collision_on_no_input = false;
	
	int dir = -1;
	++rotation_counter;
	++side_counter;

	
	// take user input for movement and rotations
	if( !DInput_Read_Keyboard() )
   {
      g_logger.logError( "DInput_Read_Keyboard()" );
      return 0;
   }

	// check if user is trying to exit
	if (keyboard_state[DIK_ESCAPE])
	{
      g_logger.logInfo( "Game Quitting...");

		quit_counter = 0;
		game_state = STATE_MENU;
		return (1);
	} // end if
	
	
	// check score
	if(  g_level <10 && g_score >= LEVEL[g_level] )
	{
      g_logger.logInfo( "Level Increased" );
		g_speed=SPEED[g_level];
		g_level++;	
      
      if( FAILED(g_sounds[SoundIDs::SND_LEVEL_ID]->Play(0)))
      {
         g_logger.logError("failed to play sound");
         return 0;
      }      
	}

	Start_Clock();						// start the timing clock
	DDraw_Fill_Surface(lpddsback, 0);	// clear the drawing surface	
	

    // draw the background 
	DDraw_Lock_Back_Surface(); 
	Draw_Bitmap16(&background_bmp, back_buffer, back_lpitch,0); 
   DDraw_Unlock_Back_Surface();  
    
	// SHIFTING STATE 
	switch (current_state)
	{
	case STATE_SHIFT:
		DoStateShift(no_input);
		break;
	case STATE_CASCADE:
		DoStateCascade();
		break;
	case STATE_NORMAL:
		DoStateNormal(no_input);	
		break;
	}	

	if( current_state != STATE_CASCADE )
	{
		// draw the falling block when necessary
      g_block->Animate();
		g_block->Draw();
	}
	// draw the preview block for the user
	g_previewBlock->Draw();
	// draw and animate the collection
	g_blockCollection->Animate();
	g_blockCollection->Draw();

	// Print game status to status-area for the user
	Draw_Text_GDI( "Score:", 15, 10, RGB(255,255,255), lpddsback );
	printInt( 70, 10, g_score, RGB(255,255,255));
	Draw_Text_GDI( "Level: ", 15, 40, RGB(255,255,255), lpddsback );
	printInt( 70, 40, g_level, RGB(255,255,255));
	Draw_Text_GDI( "Speed:", 15, 100, RGB(255,255,255), lpddsback );
	printInt( 70, 100, g_speed, RGB(255,255,255));

      /*
		thread_handle = CreateThread( NULL,                // default security
                                    0,				         // default stack 
                                    Retrieve_Name_Thread,// use this thread function
                                    (LPVOID)index,       // user data sent to thread
                                    0,				         // creation flags, 0=start now.
                                    &thread_id);	      // send id back in this var
      WaitForSingleObject( thread_handle, INFINITE );
      */
	
   static unsigned short music_volume = g_cfgManager->getEffectsVolume();
	if(keyboard_state[DIK_MINUS])
	{
      LPDIRECTSOUNDBUFFER pDSB = g_sounds[SoundIDs::SND_GAME_ID]->GetBuffer(0);
      LONG lVolume = 0;
      if( FAILED(pDSB->GetVolume(&lVolume)))
      {
         g_logger.logError("failed to get volume");
         return 0;         
      }
      if( FAILED(pDSB->SetVolume(lVolume - 100)))
      {
         g_logger.logError("failed to set volume ");
         return 0;
      }      
	}	
	if(keyboard_state[DIK_EQUALS])
	{
      LPDIRECTSOUNDBUFFER pDSB = g_sounds[SoundIDs::SND_GAME_ID]->GetBuffer(0);
      LONG lVolume = 0;
      if( FAILED(pDSB->GetVolume(&lVolume)))
      {
         g_logger.logError("failed to get volume");
         return 0;         
      }
      if( FAILED(pDSB->SetVolume(lVolume + 100)))
      {
         g_logger.logError("failed to set volume ");
         return 0;
      }    
	}	
   
    
	DDraw_Flip();						// flip the surfaces	
	Wait_Clock(30);						// sync to 30ish fps	
	return(1);							// return success

} // end Game_Main



/////////////////////////////////////////////////////////////////////////////////////
//
// prints an integer to lpddsback surface utilizing Draw_Text_GDI
//
//
/////////////////////////////////////////////////////////////////////////////////////
void printInt( int x, int y, int valToPrint, COLORREF color )
{
	char tmp[25];
	Draw_Text_GDI( itoa( valToPrint, tmp, 10 ), x, y, color, lpddsback );
}



/////////////////////////////////////////////////////////////////////////////////////
//
// Resets the Game
//
//
/////////////////////////////////////////////////////////////////////////////////////
void ResetGame()
{
	g_blockCollection->DeleteCollection();
	g_blockCollection->CreateCollection();
	
	int tmp=TYPE_DETERMINATION;
   g_block->ReloadBlock((BlockType)tmp, &BlockColors[tmp], START_X,START_Y);
	g_score = 0;
	g_level = 0;
	g_speed=SPEED[g_level];
	
	menu_up = true;
}



/////////////////////////////////////////////////////////////////////////////////////
//
// the preview block becomes the new falling block
// loads  a new preview block
//
//
/////////////////////////////////////////////////////////////////////////////////////
void LoadBlock()
{
	g_block->ReloadBlock(	g_previewBlock->GetBlockType(),
                        g_previewBlock->GetBOB(0),
						START_X,START_Y );
	
	int tmp=TYPE_DETERMINATION;
   
	g_previewBlock->ReloadBlock( (BlockType)tmp, 
                                 &BlockColors[tmp],
								         START_PREVIEW_X, 
								         START_PREVIEW_Y );
}


/////////////////////////////////////////////////////////////////////////////////////
//
// this function reads in the scores.txt file to fill in the top scores.
//
//
/////////////////////////////////////////////////////////////////////////////////////

void ReadHighScores()
{

	int score;
	char name[10];
	int num_scores;
	ifstream inFile;

	inFile.open("high_scores.txt"); // open the file

	// retrieve the number of high scores
	inFile >> num_scores;
	// retrieve each score
	for( int i=0; i< num_scores; ++i )
	{
		inFile >> name;
		inFile >> score;
		high_scores.InsertHighScore( name, score );
	}

	inFile.close(); // close file
	
}


/////////////////////////////////////////////////////////////////////////////////////
//
// writes the high scores to the scores text file
//
//
/////////////////////////////////////////////////////////////////////////////////////
void WriteHighScores()
{
	char name[10];
	int score;
	ofstream outFile;
	outFile.open("high_scores.txt"); // open file

	//	 write the number of scores
	outFile << high_scores.GetNumScores() << endl;

	// write each name/score pair
	for( int i=0; i< high_scores.GetNumScores(); ++i )
	{
		high_scores.HighScoreAtIndex( i, name, score );
		outFile << name << " ";
		outFile << score << endl;
	}

	outFile.close(); // close file
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Loads all sound effects that will be used throughout the game
//
//
/////////////////////////////////////////////////////////////////////////////////////
int LoadSounds()
{
   const char SND_GAMEBG[] = "sounds\\techno.wav";
   const char SND_MENUBG[] = "sounds\\sp066.wav";
   const char SND_LEVELUP[] = "sounds\\LEVCOMP.wav";
   const char SND_GAMEOVR[] = "sounds\\GAMEOVER.wav";
   const char SND_BLOCKROT[] = "sounds\\HWPNSEL.wav";
   const char SND_BLOCKADD[] = "sounds\\MENUSEL.wav";

   g_soundmgr = new CSoundManager;
   HRESULT hr = g_soundmgr->Initialize(main_window_handle, DSSCL_PRIORITY);
   hr = g_soundmgr->SetPrimaryBufferFormat( 2 /*channels*/, 22050 /*frequency*/, 16 /*bitrate*/ );      
   DWORD sndoptions = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |DSBCAPS_CTRLPAN;
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_GAME_ID], (LPTSTR)SND_GAMEBG, sndoptions, GUID_NULL );
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_BLOCKADD_ID], (LPTSTR)SND_BLOCKADD, sndoptions, GUID_NULL );
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_BLOCKROT_ID], (LPTSTR)SND_BLOCKROT, sndoptions, GUID_NULL );
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_GAMEOVER_ID], (LPTSTR)SND_GAMEOVR, sndoptions, GUID_NULL );
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_LEVEL_ID], (LPTSTR)SND_LEVELUP, sndoptions, GUID_NULL );
   hr = g_soundmgr->Create( &g_sounds[SoundIDs::SND_MENU_ID], (LPTSTR)SND_MENUBG, sndoptions, GUID_NULL );

   //LPDIRECTSOUNDBUFFER pDSB = sound->GetBuffer( 0 );
   //pDSB->SetVolume(-1000);
   
/*
   // Apply configuration settings 
   uint16 fxVol = g_cfgManager->getEffectsVolume();
   uint16 musVol = g_cfgManager->getMusicVolume();
   g_logger.logDebugF( "MusicVolume: %d", musVol );
   g_logger.logDebugF( "effects volume: %d", fxVol );
*/
	return (1);
}


/////////////////////////////////////////////////////////////////////////////////////
//
// NORMAL STATE  - In this state, the player is able to control the falling
// block.  He may move it around and rotate as he sees fit.//
//
/////////////////////////////////////////////////////////////////////////////////////
void DoStateNormal(int& no_input)
{
   g_logger.logDebug( "StateNormal" );
	int tmp=0;
	// if user presses down, move the block down 20 pixels (SQUARE_WIDTH)
	// and if there is a collision, reverse that move and switch to the shifting state
	if( keyboard_state[DIK_DOWN])
	{
      g_logger.logTrace( "Pressing Down" );
		g_block->MoveBlock( NO_CHANGE, DOWN, NO_CHANGE, SQUARE_WIDTH );
		if( g_blockCollection->CollisionDown( g_block ) )
		{
         g_logger.logDebug( "downward collision" );
			g_block->ColumnCorrection();
			
			current_state = STATE_SHIFT;
			shift_counter = 0;
			no_input = 0;
		}
	}
	// user is NOT pressing down at this point
	else
	{
		// does the steady fall of the block collide ?		
		g_block->MoveBlock( NO_CHANGE, DOWN, NO_CHANGE, g_speed );
		if( g_blockCollection->CollisionDown( g_block ) )
		{	
         // if so, reverse the downard move, and switch to the shifting state		
         g_logger.logDebug( "downward collision" );	
			g_block->ColumnCorrection();
			current_state = STATE_SHIFT;
			shift_counter = 0;
			no_input = 0;
		}
		// if the user is pressing up, try to rotate the block
		if( keyboard_state[DIK_UP] && rotation_counter > ROTATION_TIME)
		{
			rotation_counter = 0;

			g_block->RotateBlock(g_cfgManager->getRotDirection());
			
			// collision detection for rotations
         bool rot_collision = false;
			if( g_blockCollection->CollisionCollection( g_block ) )
         {
            rot_collision = true;
				g_block->RotateBlock( g_cfgManager->getRevRotDirection() );
         }
			else if( 0<  (tmp = g_block->GetWallProximity( WALL_NORTH ))   )
         {
            rot_collision = true;
				g_block->MoveBlock( NO_CHANGE, DOWN, NO_CHANGE, tmp );	
         }
			else if( 0<  (tmp = g_block->GetWallProximity( WALL_EAST ))   )
         {
            rot_collision = true;
				g_block->MoveBlock( LEFT, NO_CHANGE, tmp, NO_CHANGE);
         }
         else if( 0< (tmp = g_block->GetWallProximity( WALL_WEST ))   )
         {
            rot_collision = true;
				g_block->MoveBlock( RIGHT, NO_CHANGE, tmp, NO_CHANGE);	 
         }
         else if( 0< (tmp = g_block->GetWallProximity( WALL_SOUTH ))  )
         {
            rot_collision = true;
				g_block->MoveBlock( NO_CHANGE, UP, NO_CHANGE, tmp );
         }

         if( rot_collision )            
            g_logger.logDebug( "rotation Collision" );
			
			// play the rotation sound
			//DSound_Play(g_sounds[SND_BLOCKROT_ID]);
		}
		// user is pressing left, move block to left and check for collision
		if( keyboard_state[DIK_LEFT]  && side_counter > SIDE_TIME)
		{
			side_counter = 0;
			g_block->MoveBlock( LEFT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
			if( g_blockCollection->CollisionLeft( g_block ) )
         {
            g_logger.logDebug( "Leftward Collision" );
				g_block->MoveBlock( RIGHT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
         }
		}
		// user is pressing right, move block to the right and check for collision
		if( keyboard_state[DIK_RIGHT]  && side_counter > SIDE_TIME)
		{
			side_counter = 0;
			g_block->MoveBlock( RIGHT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
			if( g_blockCollection->CollisionRight( g_block ) )
         {
            g_logger.logDebug( "Rightward Collision" );
				g_block->MoveBlock( LEFT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
         }
		}			
	}	
}

/////////////////////////////////////////////////////////////////////////////////////
//
// SHIFTING STATE  - this state occurs just after a falling block makes touch down
// with either the collection or the south wall.  The player is then allowed a 
// fraction of a second to shift around the block into small crevices.  We then
// add the block to the collection and switch to the CASCADE state.//
//
/////////////////////////////////////////////////////////////////////////////////////
void DoStateShift(int& no_input)
{	
   g_logger.logDebug( "State: SHIFT" );
	int index=0;
	// to get to this state, the block has already collided with something to either
	// the south, the west, or the east:  SO NOW, we check for northern collisions
	// which signify game loss.
	if( g_blockCollection->CollisionUp( g_block ) )
	{	
      g_logger.logDebug( "Upward Collision" );
      if(FAILED(g_sounds[SoundIDs::SND_GAMEOVER_ID]->Play(0)))
         g_logger.logError("failed to play sound");
        // prompt the user for his/her name
		if( high_scores.ScoreInTopTen( g_score ) )
		{
			thread_handle = 
            CreateThread(NULL,                  // default security
                         0,						   // default stack 
                         Retrieve_Name_Thread,	// use this thread function
                         (LPVOID)index,			// user data sent to thread
                         0,						   // creation flags, 0=start now.
                         &thread_id);			   // send id back in this var

			WaitForSingleObject( thread_handle, INFINITE );
			high_scores.InsertHighScore( player_name, g_score );
		}
		
		ResetGame();
		//return;
	}

	// no_input counter is reset whenever user presses left/right to jam 
	// the block into a  small crevice.  the user has roughly a second to
	// move the block around before it is finally added.  However, if they're
	// NOT pressing left/right at all... then this time is cut, and the block
	// is immmediately added.
	++no_input;  

	// if user pressing left or right, decrement the no-input counter, 
	// and move the block appropriately
	// if there is a collision, negate the move, add the block and 
	// switch to the cascading state
	if( keyboard_state[DIK_RIGHT] )
	{
		--no_input; // there was input, decrement the no input counter
		g_block->MoveBlock( RIGHT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
		if( g_blockCollection->CollisionLeft( g_block ) )
		{
         g_logger.logDebug( "Leftward Collision" );

			g_block->MoveBlock( LEFT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
			g_score += g_blockCollection->AddBlock( g_block );
			current_state = STATE_CASCADE;
			//shift_counter = SHIFT_TIME;

			// play block-add sound effect
         g_logger.logDebug( "Upward Collision" );
         if(FAILED(g_sounds[SoundIDs::SND_BLOCKADD_ID]->Play(0)))
            g_logger.logError("failed to play sound");
		}
	}
	if( keyboard_state[DIK_LEFT] )
	{
		--no_input; // there was input, decrement the no input counter
		g_block->MoveBlock( LEFT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
		if( g_blockCollection->CollisionRight( g_block ) )
		{
         g_logger.logDebug( "Rightward Collision" );
			g_block->MoveBlock( RIGHT, NO_CHANGE, SQUARE_WIDTH, NO_CHANGE );
			g_score += g_blockCollection->AddBlock( g_block );
			current_state = STATE_CASCADE;
			//shift_counter = SHIFT_TIME;

			// play block-add sound effect
         g_logger.logDebug( "Upward Collision" );
         if(FAILED(g_sounds[SoundIDs::SND_BLOCKADD_ID]->Play(0)))
            g_logger.logError("failed to play sound");
		}
	}
	// if the user hasn't pressed lef or right for 7 frames...
	// add the block instantly.
	if( no_input > 7 || ++shift_counter > SHIFT_TIME )
	{
      g_logger.logDebug( "Adding Block to collection" );
		g_score += g_blockCollection->AddBlock( g_block );
		current_state = STATE_CASCADE;
		cascade_counter = 0;

		// play block-add sound effect
      if(FAILED(g_sounds[SoundIDs::SND_BLOCKADD_ID]->Play(0)))
         g_logger.logError("failed to play sound");
	}
	
	// attempt to fall as normal.
	// if we collide, continue in SHIFT state
	// if we do NOT collide, continue in NORMAL state
	g_block->MoveBlock( NO_CHANGE, DOWN, NO_CHANGE, g_speed );
	if( g_blockCollection->CollisionDown( g_block ) )
   {
      g_logger.logDebug( "Downward Collision" );
      g_block->MoveBlock( NO_CHANGE, UP, NO_CHANGE, g_speed );
   }
	else
		current_state = STATE_NORMAL;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// CASCADING STATE  - in the cascade state we check for complete rows within
// the collection.  If there ARE complete rows... we cascade (delete rows, and 
// make the remaining rows fall into place).//
//
/////////////////////////////////////////////////////////////////////////////////////
void DoStateCascade()
{
	if( g_blockCollection->RowHasBeenCompleted() )
	{	
		if( cascade_counter == 0 )
		{
			// animate the next row of deletion
			g_score += high_scores.ComputeScore( g_blockCollection->PrepRowDelete(), NUM_X_POSITIONS );
			g_blockCollection->SetAnimation( CASCADE );
		}
		if( ++cascade_counter >= CASCADE_TIME )
		{
         g_logger.logDebug( "deleting row" );

			// delete the current row
			g_blockCollection->Cascade();
			// animate the next row of deletion
			//score += ComputeScore(g_blockCollection->PrepRowDelete());
			g_blockCollection->PrepRowDelete();
			g_blockCollection->SetAnimation( CASCADE );
			cascade_counter = 0;	
		}
	}
	else
	{
		cascade_counter = 0;	
		current_state = STATE_NORMAL;
		LoadBlock();		
	}
}


/////////////////////////////////////////////////////////////////////////////////////
//
// determines if the mouse click (point:(x,y)) is contained within the bounds of rect
//
/////////////////////////////////////////////////////////////////////////////////////
bool MouseInRegion(int mouse_x, int mouse_y, RECT& rect)
{
	if( mouse_x < rect.left ||
		mouse_x > rect.right ||
		mouse_y < rect.top ||
		mouse_y > rect.bottom )
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// thread to collect users name
//
/////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Retrieve_Name_Thread(LPVOID data)
{
	const int TIME_BACKSPACE = 30;
	const int TIME_DELETE = 30;
	const int TIME_ARROW = 5;
	const int TIME_CURSOR = 30;
	bool draw_cursor = true;
	int cursor_timer = TIME_CURSOR;
	bool quit = false;
	int size=0;
	char character = ' ';
	bool shift_pressed = false;
	bool button_pressed = false;
	int backspace_timer = TIME_BACKSPACE;
	int delete_timer = TIME_DELETE;
	int arrow_timer = TIME_ARROW;
	CMyEditBox editBox;
	char tmp[100];
	int pressed[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int cursor_x;

	while( quit == false )
	{
		cursor_x = 350 + (editBox.GetCursorLocation()*8);
		if( ++cursor_timer >= TIME_CURSOR ) 
		{
			draw_cursor = !draw_cursor;
			cursor_timer = 0;
		}
		++backspace_timer;
		++delete_timer;
		++arrow_timer;

		Start_Clock();

		// COLLECT USER INPUT	
		DInput_Read_Keyboard();

		// IF ESCAPE PRESSED, CANCEL OUT WITHOUT SAVING A NAME
		if( keyboard_state[DIK_ESCAPE] )
		{
			quit = true;
			strcpy( player_name, "UNKOWN\0");
			continue;
		}
		// IF ENTER PRESSED, SAVE GAME SCORE/NAME AND EXIT LOOP
		else if( keyboard_state[DIK_RETURN] )
		{
			quit = true;
			strcpy( player_name, editBox.GetString());
			enter_counter =0;
			continue;
		}

		DDraw_Lock_Back_Surface();  // lock the back buffer for writing
        Draw_Bitmap16(&background_bmp, back_buffer, back_lpitch,0);
		Draw_Bitmap16(&popup_bmp, back_buffer, back_lpitch,0 );
		// draw the cursor
		if( draw_cursor )
			Draw_Line16( cursor_x ,260,cursor_x,275, RGB(255,255,255), back_buffer, back_lpitch );
		DDraw_Unlock_Back_Surface();  // unlock back buffer

		// draw to the screen a message that informs player that he has won a 
		// high score and needs to input his name for the score board
		Draw_Text_GDI( "Congratulations!\0",
			160, 150, RGB(255,255,255), lpddsback );
		Draw_Text_GDI( "Your score has made the top 10 list!\0",
			160, 170, RGB(255,255,255), lpddsback );
		Draw_Text_GDI( "Enter your name and be remembered...\0", 
			160, 235, RGB(255,255,255), lpddsback );
		Draw_Text_GDI( "Elite tetrix Player:\0", 
			160, 260, RGB(255,255,255), lpddsback );

		// see if shift is being pressed
		if( keyboard_state[DIK_RSHIFT] || keyboard_state[DIK_LSHIFT] )
			shift_pressed = true;
		

		// if user pressed backspace, delete previous character
		if( keyboard_state[DIK_BACK]   )
		{	
			if(backspace_timer >= TIME_BACKSPACE) 
			{editBox.DeletePreviusChar(); backspace_timer = 0; draw_cursor = true;}
			
		}
		else backspace_timer = TIME_BACKSPACE;

		// if user presses delete key, delete current character
		if( keyboard_state[DIK_DELETE] )
		{
			if( delete_timer >= TIME_DELETE )
				{
					editBox.DeleteCurrentChar(); delete_timer = 0;draw_cursor = true;}			
		}
		else
			delete_timer = TIME_DELETE;

		// if user presses left or right key... move cursor over respectively
		if( keyboard_state[DIK_LEFT] )
		{
			if( arrow_timer >= TIME_ARROW )
				{editBox.MoveCursorLeft(); arrow_timer = 0; draw_cursor = true;}
		}
		else
			arrow_timer = TIME_ARROW;
		if( keyboard_state[DIK_RIGHT] )
		{
			if( arrow_timer >= TIME_ARROW )
				{editBox.MoveCursorRight(); arrow_timer = 0; draw_cursor = true;}
		}
		else
			arrow_timer = TIME_ARROW;




		// test all the letter keys on the keyboard
		if( !button_pressed && keyboard_state[DIK_A] ) 
		{
			if( !pressed[0] )
				ButtonActivate( 'a', pressed, &character, &button_pressed );
		} 
		else
			pressed[0] = false;
		if( !button_pressed && keyboard_state[DIK_B] ) 
		{
			if( !pressed[1] )
				ButtonActivate( 'b', pressed, &character, &button_pressed );
		} 
		else
			pressed[1] = false;
		if( !button_pressed && keyboard_state[DIK_C] ) 
		{
			if( !pressed[2] )
				ButtonActivate( 'c', pressed, &character, &button_pressed );
		} 
		else
			pressed[2] = false;
		if( !button_pressed && keyboard_state[DIK_D] ) 
		{
			if( !pressed[3] )
				ButtonActivate( 'd', pressed, &character, &button_pressed );
		} 
		else
			pressed[3] = false;
		if( !button_pressed && keyboard_state[DIK_E] ) 
		{
			if( !pressed[4] )
				ButtonActivate( 'e', pressed, &character, &button_pressed );
		} 
		else
			pressed[4] = false;
		if( !button_pressed && keyboard_state[DIK_F] ) 
		{
			if( !pressed[5] )
				ButtonActivate( 'f', pressed, &character, &button_pressed );
		} 
		else
			pressed[5] = false;
		if( !button_pressed && keyboard_state[DIK_G] ) 
		{
			if( !pressed[6] )
				ButtonActivate( 'g', pressed, &character, &button_pressed );
		} 
		else
			pressed[6] = false;
		if( !button_pressed && keyboard_state[DIK_H] ) 
		{
			if( !pressed[7] )
				ButtonActivate( 'h', pressed, &character, &button_pressed );
		} 
		else
			pressed[7] = false;
		if( !button_pressed && keyboard_state[DIK_I] ) 
		{
			if( !pressed[8] )
				ButtonActivate( 'i', pressed, &character, &button_pressed );
		} 
		else
			pressed[8] = false;
		if( !button_pressed && keyboard_state[DIK_J] ) 
		{
			if( !pressed[9] )
				ButtonActivate( 'j', pressed, &character, &button_pressed );
		} 
		else
			pressed[9] = false;
		if( !button_pressed && keyboard_state[DIK_K] ) 
		{
			if( !pressed[10] )
				ButtonActivate( 'k', pressed, &character, &button_pressed );
		} 
		else
			pressed[10] = false;
		if( !button_pressed && keyboard_state[DIK_L] ) 
		{
			if( !pressed[11] )
				ButtonActivate( 'l', pressed, &character, &button_pressed );
		} 
		else
			pressed[11] = false;
		if( !button_pressed && keyboard_state[DIK_M] ) 
		{
			if( !pressed[12] )
				ButtonActivate( 'm', pressed, &character, &button_pressed );
		} 
		else
			pressed[12] = false;
		if( !button_pressed && keyboard_state[DIK_N] ) 
		{
			if( !pressed[13] )
				ButtonActivate( 'n', pressed, &character, &button_pressed );
		} 
		else
			pressed[13] = false;
		if( !button_pressed && keyboard_state[DIK_O] ) 
		{
			if( !pressed[14] )
				ButtonActivate( 'o', pressed, &character, &button_pressed );
		} 
		else
			pressed[14] = false;
		if( !button_pressed && keyboard_state[DIK_P] ) 
		{
			if( !pressed[15] )
				ButtonActivate( 'p', pressed, &character, &button_pressed );
		} 
		else
			pressed[15] = false;
		if( !button_pressed && keyboard_state[DIK_Q] ) 
		{
			if( !pressed[16] )
				ButtonActivate( 'q', pressed, &character, &button_pressed );
		} 
		else
			pressed[16] = false;
		if( !button_pressed && keyboard_state[DIK_R] ) 
		{
			if( !pressed[17] )
				ButtonActivate( 'r', pressed, &character, &button_pressed );
		} 
		else
			pressed[17] = false;
		if( !button_pressed && keyboard_state[DIK_S] ) 
		{
			if( !pressed[18] )
				ButtonActivate( 's', pressed, &character, &button_pressed );
		} 
		else
			pressed[18] = false;
		if( !button_pressed && keyboard_state[DIK_T] ) 
		{
			if( !pressed[19] )
				ButtonActivate( 't', pressed, &character, &button_pressed );
		} 
		else
			pressed[19] = false;
		if( !button_pressed && keyboard_state[DIK_U] ) 
		{
			if( !pressed[20] )
				ButtonActivate( 'u', pressed, &character, &button_pressed );
		} 
		else
			pressed[20] = false;
		if( !button_pressed && keyboard_state[DIK_V] ) 
		{
			if( !pressed[21] )
				ButtonActivate( 'v', pressed, &character, &button_pressed );
		} 
		else
			pressed[21] = false;
		if( !button_pressed && keyboard_state[DIK_W] ) 
		{
			if( !pressed[22] )
				ButtonActivate( 'w', pressed, &character, &button_pressed );
		} 
		else
			pressed[22] = false;
		if( !button_pressed && keyboard_state[DIK_X] ) 
		{
			if( !pressed[23] )
				ButtonActivate( 'x', pressed, &character, &button_pressed );
		} 
		else
			pressed[23] = false;
		if( !button_pressed && keyboard_state[DIK_Y] ) 
		{
			if( !pressed[24] )
				ButtonActivate( 'y', pressed, &character, &button_pressed );
		} 
		else
			pressed[24] = false;
		if( !button_pressed && keyboard_state[DIK_Z] ) 
		{
			if( !pressed[25] )
				ButtonActivate( 'z', pressed, &character, &button_pressed );
		} 
		else
			pressed[25] = false;

		// add the character
		if( button_pressed )
		{	
			editBox.AddChar( character - (shift_pressed ? 32 : 0));
			draw_cursor = true;
		}
		strcpy( tmp, editBox.GetString() );
		
		// draw out the buffer
		Draw_Text_GDI( tmp, 350, 260, RGB(255,255,255), lpddsback );

		button_pressed = false;
		shift_pressed = false;

		DDraw_Flip();	

		Wait_Clock(30);	
	}

	return ((DWORD)data);
}

//helper function.
void ButtonActivate( char button, int* pressed, char* character, bool* button_pressed)
{
	//convert the passed in character to an integer and index it into the pressed array
	//ensure that the button has been pressed
	pressed[((int)button -97)] = true; 
	*character = button; 
	*button_pressed = true;
}

void MyAlphaBlend(USHORT *tbuffer, BITMAP_IMAGE* alpha_dest, USHORT *s1buffer, USHORT *s2buffer, float alphaf)
{
	if( !s1buffer || !s2buffer || !tbuffer || ! alpha_dest )
		return;

	for (int iy = 0; iy < alpha_dest->height; iy++)
	{
		for (int ix = 0; ix < alpha_dest->width; ix++)
        {
			int rs1, gs1, bs1;   // used to extract the source rgb values
			int rs2, gs2, bs2; // light map rgb values
			int rf,  gf,  bf;   // the final rgb terms

			// extract pixel from source bitmap
			USHORT s1pixel = s1buffer[iy*alpha_dest->width + ix];

			// extract RGB values
			_RGB565FROM16BIT(s1pixel, &rs1,&gs1,&bs1);
			// extract pixel from lightmap bitmap
			USHORT s2pixel = s2buffer[iy*alpha_dest->width + ix];

			// extract RGB values
			_RGB565FROM16BIT(s2pixel, &rs2,&gs2,&bs2);

			// alpha blend them
			rf = (alphaf * (float)rs1 ) + ((1 - alphaf) * (float)rs2 );
			gf = (alphaf * (float)gs1 ) + ((1 - alphaf) * (float)gs2 );
			bf = (alphaf * (float)bs1 ) + ((1 - alphaf) * (float)bs2 );

			// test for overflow
			if (rf > 255) rf=255;
			if (gf > 255) gf=255;
			if (bf > 255) bf=255;

			// rebuild RGB and test for overflow
			// and write back to buffer
			tbuffer[iy*alpha_dest->width + ix] = _RGB16BIT565(rf,gf,bf);
	         
         } // end for ix   
	} // end for iy
}