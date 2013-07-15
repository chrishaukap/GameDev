#include "stdafx.h"
#include "thinkTask.h"
#include "TTTAIUtils.h"
#include "TTTGameState.h"
#include "myTimer.h"
#include "thread.h"
#include "TTTGrid.h"

using namespace CDH;
using namespace TicTacToe;

static const char filename[] = "ThinkTask.txt";


TaskObject::~TaskObject()
{
   delete m_grid; m_grid = NULL;
}

ThinkTask::ThinkTask(TaskObject* taskObject,
                     CHUint depth) :
   m_taskObject(taskObject),
   m_depth(depth),
   m_mytimer(new mytimer())
{}

ThinkTask::~ThinkTask()
{
   delete m_taskObject;
   delete m_mytimer;
}

static void writeTimeToFile(DWORD tickcount)
{
   CDH::Mutex mutex;
   mutex.enterCriticalSection();
   {
      FILE* file = fopen(filename, "a");	   
	   char buf[100];
	   sprintf( buf, "tickCount: %d\n", tickcount );
	   fwrite(buf, strlen(buf), sizeof(char), file);
	   fclose(file);
   }
   mutex.exitCriticalSection();
}

Status 
ThinkTask::doTask()
{
   //m_mytimer->start();
   
   NodeState nodeToBeat(INT_MAX, TTTPlayerMove(0,0,Invalid));
   NodeState enemyMove = TTTAIUtils::getMinMove( *(m_taskObject->m_grid), 
                                                 m_taskObject->m_theMove.m_move, 
                                                 m_taskObject->m_turn, 
                                                 nodeToBeat, m_depth, false);
   m_taskObject->m_theMove.m_value = enemyMove.m_value;
   assert(enemyMove.m_move.m_turn != Invalid);
   
   //m_mytimer->end();
   //writeTimeToFile(m_mytimer->tickCount());

   return StatusSuccess;
}