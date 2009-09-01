
#include "Game.h"
#include "World.h"
#include "MathUtil.h"
#include "BasicFastDispatcher.h"
#include "collisions.h"
#include "Cole.h"

#include <assert.h>
using namespace CDH;
using namespace Cole;

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

void 
Game::start()
{
	theWorld.SetupPhysics();
	m_cole = new ColeAvatar;
	m_cole->Init();

	m_ground = new PhysicsActor();
	m_ground->SetPosition(0.0f, -11.0f);
	m_ground->SetSize(80.0f, 5.0f);
	m_ground->SetColor(0.0f, 1.0f, 0.0f);
	m_ground->InitPhysics(0.0f, 0.1f); //no density (static) little friction
	theWorld.Add( m_ground );
}
void 
Game::stop()
{
	m_cole->Clear();
	delete m_cole; m_cole = NULL;
	theWorld.Remove( m_ground );
	delete m_ground; m_ground = NULL;
}

void Game::cleanup()
{ 
}
void
Game::update(float dt)
{  
}