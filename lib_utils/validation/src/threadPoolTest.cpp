#include "threadPoolTest.h"
#include "threadPool.h"
#include <stdio.h>
using namespace CDH;

#define NUMTHREADS 4
#define NUMTASKS 400
class MyTask : public ThreadTask
{
public:
   MyTask(CHUint taskNumber) : ThreadTask(), m_num(taskNumber) {}
   Status doTask()
   {
      fprintf(stderr, "Task %d sleeping...\n", m_num);
      Sleep(100);
      fprintf(stderr, "Task %d running...\n", m_num);
      for(CHUint i=0; i<m_num; ++i)
         fprintf(stderr, "[%d] %d\n", m_num, i);
      fprintf(stderr, "Task %d sleeping...\n", m_num);
      Sleep(100);
      return StatusSuccess;
   }
private:
   MyTask();
   CHUint m_num;
};


ThreadPoolTest::ThreadPoolTest(){}
ThreadPoolTest::~ThreadPoolTest(){}

void
ThreadPoolTest::test1()
{
	{
		const CHUint numThreads = NUMTHREADS;
		ThreadPool pool(numThreads);
		pool.run();
		const CHUint numTasks = NUMTASKS;
		MyTask *tasks[numTasks];
		for(CHUint i=0; i<numTasks; ++i)
		{
			tasks[i] = new MyTask(i+1);
			pool.addTask(tasks[i]);
		}
		pool.finishTasks();
		pool.killAllThreads();

		for(CHUint i=0; i<numTasks; ++i)
			delete tasks[i];

		fprintf(stderr, "ALL THREADS DONE\n");
	}
}

void
ThreadPoolTest::run()
{
   test1();
}