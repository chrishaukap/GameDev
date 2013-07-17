#pragma once
#include "Infrastructure/GameManager.h"
#include "Infrastructure/Console.h"

#include <vector>
#include <string>

class GameScreen : public Renderable
{
public:
	GameScreen(){}

   virtual void Start(){}
	virtual void Stop();

	virtual void Update(float dt){}
	virtual void Render(){}

protected:
	std::vector<Renderable*> m_objects;
};

#define theTTTGameMngr TicTacToeGameManager::getInstance()
class TicTacToeGameManager : public GameManager
{
public:
	void MoveForwards(const String& input);
	void Restart(const String& input);
   void init(unsigned int width, unsigned int height);
   unsigned int getWindowWidth() const {return m_windowWidth;}
   unsigned int getWindowHeight() const {return m_windowHeight;}

	static TicTacToeGameManager &getInstance();
	virtual void Render();
	virtual void Update(float dt);
protected:
   
	TicTacToeGameManager();
	static TicTacToeGameManager *s_TicTacToeGameManager;

private:
   unsigned int m_windowWidth;
   unsigned int m_windowHeight;
   enum ScreenType { ScreenTypeTicTacToe = 0, ScreenTypeVictory};
	std::vector<GameScreen*> m_screens;
	unsigned int m_currentScreen;
};
