
#include "Game.h"
#include "World.h"
#include "MathUtil.h"
#include "BasicFastDispatcher.h"
#include "collisions.h"
#include "jake.h"

#include <assert.h>
using namespace CDH;
using namespace Chubdiso;

namespace 
{
   BasicFastDispatcher<iCollidable> g_basicFastDispatcher;
   bool g_initted = false;
}

Game::Game(CHUint rows, CHUint cols)
{
   if(!g_initted)
   {
      g_initted = true;
   }
}
Game::~Game()
{}

static Actor* g_ground = NULL;
void 
Game::start()
{
	theWorld.SetupPhysics();
	m_jake = new Jake;
	m_jake->Init();

	g_ground = new Actor;
	g_ground->SetPosition(0,-7);
	g_ground->SetSize(27, 7);
	g_ground->SetColor(0,1,0);
	theWorld.Add(g_ground,0);
}
void 
Game::stop()
{
	m_jake->Clear();
	delete m_jake; m_jake = NULL;

	theWorld.Remove(g_ground);
	delete g_ground; g_ground = NULL;
}

void Game::cleanup()
{ 
}
void
Game::update(float dt)
{  
}