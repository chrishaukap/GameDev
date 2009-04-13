#pragma once
#include "GameManager.h"
#include "Console.h"

#include <vector>
#include <string>

class GameScreen : public Renderable
{
public:
	GameScreen();

	virtual void Start();
	virtual void Stop(); // removes all objects from world, but does not delete them
   virtual void Kill(); // done with the screen, kill it... 

	virtual void Update(float dt);
	virtual void Render();

protected:
	std::vector<Renderable*> m_objects;
};


//TheEndGameManager is a singleton and inherits from:
//  GameManager -- so we receive update and render calls, plus collision notifications
//  CCmdMemberFunctorOwner -- so we can define console commands to use internal functions
#define theEndGame TheEndGameManager::getInstance()

class TheEndGameManager : public GameManager, public CCmdMemberFunctorOwner
{
public:
	void MoveForwards(const String& input);
	void MoveBackwards(const String& input);

	static TheEndGameManager &getInstance();
	virtual void Render();

protected:
	TheEndGameManager();
	static TheEndGameManager *s_TheEndGameManager;

private:
   enum ScreenType
   {
      CutScene = 0,
      Interactive,
      GainLoss
   };

	std::vector<GameScreen*> m_screens;
	unsigned int m_currentScreen;
};
