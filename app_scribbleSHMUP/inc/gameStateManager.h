#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "cdhBase.h"

namespace CDH
{

class CGfxOpenGL;
class GameEngine;
class GameStateManager
{
public:
   GameStateManager(CGfxOpenGL& renderer, GameEngine& engine);
	~GameStateManager();

   Status initialize();
   void process();
   void render();
   void menu();
   void run();
   void credits();
   bool inMenu() const;
   bool running() const;
   bool inCredits() const;

private:   
   enum GameState
   {
      StateInit,
      StateMenu,
      StateRunning,
      StateCredits,
      StateInvalid
   };

   GameState m_state;
   CGfxOpenGL &m_renderer;
   GameEngine &m_engine;

   GameStateManager(const GameStateManager& C);
   const GameStateManager& operator=(const GameStateManager& C);
};

}


#endif // GAMESTATEMANAGER_H