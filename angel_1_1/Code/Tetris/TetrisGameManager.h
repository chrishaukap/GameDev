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
	virtual void Stop(); //calls remove on all _objects and deletes them
	virtual void Update(float dt);
	virtual void Render();

protected:
	std::vector<Renderable*> _objects;
};

#define theTetrisGame TetrisGameManager::getInstance()

class TetrisGameManager : public GameManager, public CCmdMemberFunctorOwner
{
public:
   void AddActor(const String& input);
   void RemoveActor(const String& input);

	void MoveForwards(const String& input);
	void MoveBackwards(const String& input);
	static TetrisGameManager &getInstance();
	virtual void Render();

protected:
	TetrisGameManager();
	static TetrisGameManager *s_TetrisGameManager;

private:
	std::vector<GameScreen*> _screens;
	int _current;
};
