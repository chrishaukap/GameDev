#pragma once
#include "GameManager.h"
#include "Console.h"

#include <vector>
#include <string>

class DemoScreen : public Renderable
{
public:
	DemoScreen();

	virtual void Start();
	virtual void Stop(); //calls remove on all _objects and deletes them
	virtual void Update(float dt);
	virtual void Render();

protected:
	std::vector<Renderable*> _objects;
};


//DemoGameManager is a singleton and inherits from:
//  GameManager -- so we receive update and render calls, plus collision notifications
//  CCmdMemberFunctorOwner -- so we can define console commands to use internal functions
#define theDemoGame DemoGameManager::getInstance()

class DemoGameManager : public GameManager, public CCmdMemberFunctorOwner
{
public:
   void AddActor(const String& input);
   void RemoveActor(const String& input);

	void MoveForwards(const String& input);
	void MoveBackwards(const String& input);

	static DemoGameManager &getInstance();
	virtual void Render();

protected:
	DemoGameManager();
	static DemoGameManager *s_DemoGameManager;

private:
	std::vector<DemoScreen*> _screens;
	int _current;
};
