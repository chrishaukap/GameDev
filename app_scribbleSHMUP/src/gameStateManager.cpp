#include "gameStateManager.h"
#include "gameEngine.h"
#include "cgfxopengl.h"
#include <assert.h>

using namespace CDH;

GameStateManager::GameStateManager(CGfxOpenGL& renderer, GameEngine& engine) :
   m_state(StateInit),
   m_renderer(renderer),
   m_engine(engine)
{}   
GameStateManager::~GameStateManager()
{}

Status 
GameStateManager::initialize()
{
   switch(m_state)
   {
   case StateInit: 
      if( !m_renderer.Init() )
			return StatusFailure;
      m_engine.initializePreMenu();
      menu();
      break;
   case StateMenu: 
   case StateRunning: 
   case StateCredits: 
   case StateInvalid:
   default:
      assert(!"INVALID STATE");
      break;      
   }
	return StatusSuccess;
}
void 
GameStateManager::process()
{
   switch(m_state)
   {
   case StateInit: assert(!"NOT INITIALIZED"); break;
   case StateMenu: 
      break;
   case StateRunning: 
      m_engine.process(); 
      break;
   case StateCredits: 
      break;
   case StateInvalid:
   default:
      assert(!"INVALID STATE");
      break;
   }
}
void 
GameStateManager::render()
{
   switch(m_state)
   {
   case StateInit: assert(!"NOT INITIALIZED"); break;
   case StateMenu: 
      m_renderer.renderMenu();
      break;
   case StateRunning:
      m_renderer.render();
      break;
   case StateCredits:
      m_renderer.renderCreditScreen();
      break;
   case StateInvalid:
   default:
      assert(!"INVALID STATE");
      break;
   }
}
void 
GameStateManager::menu() 
{
	m_state = StateMenu;
}
void 
GameStateManager::run() 
{
   static CHUint timesCalled = 0;
   assert( ++timesCalled == 1 );

   m_engine.initializePostMenu();
   m_state = StateRunning;
}
void 
GameStateManager::credits() 
{
	m_state = StateCredits;
}
bool 
GameStateManager::inMenu() const 
{
	return m_state == StateMenu;
}
bool 
GameStateManager::running() const 
{
	return m_state == StateRunning;
}
bool 
GameStateManager::inCredits() const 
{
	return m_state == StateCredits;
}