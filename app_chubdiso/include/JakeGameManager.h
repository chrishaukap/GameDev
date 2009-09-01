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
   virtual void CleanUp(){}
protected:
	std::vector<Renderable*> _objects;
};

#define theJakeGame JakeGameManager::getInstance()

class JakeGameManager : public GameManager, public CCmdMemberFunctorOwner
{
public:
	bool isADown() const;
	bool isDDown() const;
	bool isWDown() const;
	void W_Down(const String& input);
	void A_Down(const String& input);
	void D_Down(const String& input);
	void W_Up(const String& input);
	void A_Up(const String& input);
	void D_Up(const String& input);
	void MoveForwards(const String& input);
	void MoveBackwards(const String& input);
	static JakeGameManager &getInstance();
	virtual void Render();
   virtual void Update(float dt);   
   virtual void CleanUp();

protected:
	JakeGameManager();
	static JakeGameManager *s_JakeGameManager;

private:
	std::vector<GameScreen*> _screens;
	int _current;
};
