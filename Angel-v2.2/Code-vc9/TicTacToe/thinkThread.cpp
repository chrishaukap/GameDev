#include "stdafx.h"
#include "thinkThread.h"
#include "TTTAIUtils.h"
#include "TTTGameState.h"

using namespace CDH;
using namespace TicTacToe;


ThinkThread::ThinkThread() :
	m_gameState(NULL),
	m_depth(0),
	m_theirBest(0),
   m_state(Idle)
{
}
ThinkThread::~ThinkThread()
{
	delete m_gameState;
}
void ThinkThread::preExecute(const TTTGameState& gameState, int theirBest, unsigned int depth)
{	
	assert( m_state != Running );

	delete m_gameState;
	m_gameState = new TTTGameState(gameState);
	m_theirBest = theirBest;
	m_depth = depth;
	m_state = ReadyToExecute;
	m_theMove = TTTMovesList::TTTMoveValuePair();
}
bool ThinkThread::isReady() const
{
   return bool(m_state == ReadyToExecute);
}
bool ThinkThread::isThinking() const
{
	return bool(m_state == Running);
}
bool ThinkThread::isIdle() const
{
	return bool(m_state == Idle);
}
void ThinkThread::think()
{
   m_state = Running;
}
void ThinkThread::stop()
{
	interrupt();
}
Status ThinkThread::task()
{
	assert( isIdle() );
	while( !shouldStop() )
	{
		switch(m_state)
		{
      case Running:			
			m_theMove = TTTAIUtils::getMinMove(*m_gameState, m_theirBest, m_depth);
			m_state = Idle;
			break;
		case ReadyToExecute:
		case Idle:
			Sleep(300);
			break;
		default: assert( !"invalid state" );
		}
	}
	return StatusSuccess;
}
