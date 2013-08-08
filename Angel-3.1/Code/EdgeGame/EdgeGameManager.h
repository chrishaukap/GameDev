#pragma once
#include "Infrastructure/GameManager.h"
#include "Infrastructure/Console.h"

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
   virtual void CleanUp(){}
protected:
	std::vector<Renderable*> _objects;
};

#define theEdgeGame EdgeGameManager::getInstance()

class EdgeGameManager : public GameManager
{
public:
   void AddActor(const String& input);
   void RemoveActor(const String& input);

	void MoveForwards(const String& input);
	void MoveBackwards(const String& input);
	static EdgeGameManager &getInstance();
	virtual void Render();
   virtual void Update(float dt);   
   virtual void CleanUp();

protected:
	EdgeGameManager();
	static EdgeGameManager *s_EdgeGameManager;

private:
	std::vector<GameScreen*> _screens;
	int _current;
};
