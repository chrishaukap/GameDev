#include "StdAfx.h"
#include "Game.h"
#include "Indie.h"
#include "World.h"

#include <assert.h>
using namespace CDH;
using namespace Edge;

Game::Game(CHUint rows, CHUint cols) :
	m_indie()
{}
Game::~Game()
{}

void 
Game::start()
{
	for(CDH::CHUint i=0; i< 100; ++i)
	{
		Indie* indie = new Indie();
		indie->SetSize(1.0f);
		indie->SetPosition(0,0);
		indie->SetColor(0,0,0);
		theWorld.Add(indie);
		m_indie.push_back( indie );
	}

}
void 
Game::stop()
{
	std::vector<Indie*>::iterator iter = m_indie.begin();
	while( iter != m_indie.end() )
	{
		theWorld.Remove( *iter );
		delete (*iter);
		++iter;
	}
	m_indie.clear();
}
void
Game::update(float dt)
{
	std::vector<Indie*>::iterator iter = m_indie.begin();
	while( iter != m_indie.end() )
	{
		(*iter)->Update(dt);
		++iter;
	}
}