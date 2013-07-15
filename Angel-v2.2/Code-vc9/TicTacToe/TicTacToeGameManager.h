#pragma once
#include "GameManager.h"
#include "Console.h"

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
class TicTacToeGameManager : public GameManager/*, public CCmdMemberFunctorOwner*/
{
public:
	void MoveForwards(const String& input);
	void Restart(const String& input);

	static TicTacToeGameManager &getInstance();
	virtual void Render();
	virtual void Update(float dt);
protected:
   
	TicTacToeGameManager();
	static TicTacToeGameManager *s_TicTacToeGameManager;

private:
   enum ScreenType { ScreenTypeTicTacToe = 0, ScreenTypeVictory};
	std::vector<GameScreen*> m_screens;
	unsigned int m_currentScreen;
};
