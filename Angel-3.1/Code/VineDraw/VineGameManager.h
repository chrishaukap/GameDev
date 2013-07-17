#pragma once
#include "Infrastructure/GameManager.h"
#include "Infrastructure/Console.h"

#include <vector>
#include <string>

#define theVineGame VineGameManager::getInstance()

class GameScreen;
class VineGameManager : public GameManager
{
public:
	void MoveForwards(const String& input);
	void Restart(const String& input);

	static VineGameManager &getInstance();
	virtual void Render();
	virtual void Update(float dt);
protected:
   
	VineGameManager();
	static VineGameManager *s_VineGameManager;

private:
   enum ScreenType { ScreenTypeVine = 0 };
	std::vector<GameScreen*> m_screens;
	unsigned int m_currentScreen;
};
