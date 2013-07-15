#include "StdAfx.h"
#include "TicTacToeGameManager.h"
#include "TicTacToe.h"
#include "TextRendering.h"
#include "TextActor.h"

#include "World.h"
using namespace TicTacToe;

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define GRID_SIZE 19
#define PLAYER1_IS_AI true
#define PLAYER2_IS_AI true

void GameScreen::Stop()
{
	std::vector<Renderable*>::iterator it = m_objects.begin();
	while(m_objects.end() != it)
	{
		theWorld.Remove(*it);
      delete *it;
		it++;
	}
   m_objects.clear();
}

class TicTacToeScreen : public GameScreen
{
public:
   TicTacToeScreen(unsigned int width, unsigned int height, unsigned gridSize) : 
      m_screenWidth(width), m_screenHeight(height), 
      m_gridSize(gridSize) 
   {}
   ~TicTacToeScreen() {theTTTGame.Destroy();}

   virtual void Start()
   {
	   Vector2 fULCorner = MathUtil::ScreenToWorld(0, 0);
	   Vector2 fLRCorner = MathUtil::ScreenToWorld(m_screenWidth, m_screenHeight);
	   Vector2 scrnDims( fabs(fLRCorner.X) + fabs(fULCorner.X), fabs(fLRCorner.Y) + fabs(fULCorner.Y) );

	   float multiplier = 0.8f;
	   float minGridDim = multiplier * MIN(scrnDims.X, scrnDims.Y);

	   float cellSize = minGridDim / float(m_gridSize);	
      theTTTGame.StartGame(cellSize, m_gridSize);    
		theTTTGame.setPlayer1Type(PLAYER1_IS_AI);
		theTTTGame.setPlayer2Type(PLAYER2_IS_AI);
		
		TextActor *actor = new TextActor("Console", "Press SPACE to restart");
		actor->SetColor(0,0,0);
		actor->SetPosition(-6, 9);
		actor->SetAlignment(TXT_Center);
		theWorld.Add(actor,3);
		m_objects.push_back(actor);
   }
	virtual void Stop()
   {
      theTTTGame.EndGame();
		GameScreen::Stop();
   }
private:
   TicTacToeScreen();
   unsigned int m_screenWidth, m_screenHeight;
   unsigned int m_gridSize;
};

class VictoryScreen : public GameScreen
{
public:
   VictoryScreen() : 
      m_square()
   {}
   ~VictoryScreen() {}

   virtual void Start()
   {
      if( theTTTGame.GetTurn() == Invalid )
         m_square.SetColor( Color(0,0,1) );
      else if( theTTTGame.GetTurn() == X )
         m_square.SetColor( Color(1,0,0) );
      else
         m_square.SetColor( Color(0,1,0) );
      theWorld.Add( &m_square );
		
		TextActor *actor = new TextActor("Console", "Press SPACE to restart");
		actor->SetColor(0,0,0);
		actor->SetPosition(-6, 9);
		actor->SetAlignment(TXT_Center);
		theWorld.Add(actor,3);
		m_objects.push_back(actor);
   }
	virtual void Stop()
   {
		GameScreen::Stop();
   }
private:
   Actor m_square;
};

TicTacToeGameManager::TicTacToeGameManager()
{
	//CONSOLE_DECLARECMDMETHOD(Restart, this, TicTacToeGameManager, Restart);

   unsigned int width = 1600, height = 900;
   //theWorld.get.getWindowDimensions(width,height);
	m_screens.push_back(new TicTacToeScreen(width,height,GRID_SIZE));    
   m_screens.push_back(new VictoryScreen);

   theWorld.Add(m_screens[ScreenTypeTicTacToe]);
   m_screens[ScreenTypeTicTacToe]->Start();
	m_currentScreen = ScreenTypeTicTacToe;
}
TicTacToeGameManager* TicTacToeGameManager::s_TicTacToeGameManager = NULL;
TicTacToeGameManager& TicTacToeGameManager::getInstance()
{
	if (s_TicTacToeGameManager == NULL)
	{
		s_TicTacToeGameManager = new TicTacToeGameManager();
	}
	return *s_TicTacToeGameManager;
}

void TicTacToeGameManager::MoveForwards(const String& input)
{
   unsigned int numScreens = (unsigned int)m_screens.size();
	if ((m_currentScreen >= 0) && (m_currentScreen < (numScreens-1)))
	{
		m_screens[m_currentScreen]->Stop();
		theWorld.Remove(m_screens[m_currentScreen]);

      ++m_currentScreen;
		m_screens[ m_currentScreen ]->Start();
		theWorld.Add(m_screens[m_currentScreen]);
	}
}

void TicTacToeGameManager::Restart(const String& input)
{
	m_screens[m_currentScreen]->Stop();
   m_screens[ScreenTypeTicTacToe]->Start();
	m_currentScreen = ScreenTypeTicTacToe;
}

void TicTacToeGameManager::Update(float dt)
{
   if(m_currentScreen == ScreenTypeTicTacToe)
      theTTTGame.Process();
	if(m_currentScreen == ScreenTypeTicTacToe &&
		theTTTGame.GameOver())
	{
		String str;
		theTTTGameMngr.MoveForwards(str);
	}	
}
void TicTacToeGameManager::Render()
{
	//glColor(0.5f, 0.5f, 0.5f);
	char* infoString = "";
	int xOffset = 900;
	DrawGameText(infoString, "ConsoleSmall", xOffset, 763);
}