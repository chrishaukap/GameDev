#include "threadPool.h"
#include "thread.h"
#include <list>
#include <vector>
#include <assert.h>

namespace CDH
{
	class WorkerThread : public Thread
	{
	public:
		WorkerThread(ThreadPool& myPool, HANDLE workToDoEvent);
		virtual ~WorkerThread();
		virtual Status task();
      bool isFinished() const {return m_state == Idle;}
      bool isWorking() const {return m_state == Working;}
	private:
      enum WorkerState
      {
         Idle =0, Working
      };
		ThreadPool& m_myPool;
		HANDLE m_workToDo;
      WorkerState m_state;

		WorkerThread();
		WorkerThread(const WorkerThread& C);
		const WorkerThread& operator=(const WorkerThread& C);				
	};
}
using namespace CDH;

WorkerThread::WorkerThread(ThreadPool& myPool, HANDLE workToDoEvent) : 
	m_myPool(myPool),
	m_workToDo(workToDoEvent),
   m_state(Idle)
{}
WorkerThread::~WorkerThread()
{}

#define WAIT_FOR(event,time)							\
	switch(::WaitForSingleObject(event, time))	\
	{															\
		case WAIT_OBJECT_0: break;						\
		case WAIT_TIMEOUT: continue;					\
	}

Status 
WorkerThread::task()
{
	while(!shouldStop())
	{
		WAIT_FOR(m_workToDo,1000)
		{
         m_state = Working;
		   ThreadTask* task = m_myPool.claimNextTask();
         if(task)
         {
		      Status taskStatus = task->doTask();
		      if(StatusFailed(taskStatus))
		      {
			      // HAUKAP - ?
		      }
         }
         m_state = Idle;
		}
	}
	return StatusSuccess;
}

class ThreadPool::Threads
{
public:
	Threads() : m_threads() {}
	~Threads()
	{
		CHUint sz = (CHUint)m_threads.size();
		for( CHUint i=0; i<sz; ++i )
			delete m_threads[i];
		m_threads.clear();
	}
	std::vector<WorkerThread*> m_threads;
private:
	Threads(const Threads& C);
	const Threads& operator=(const Threads& C);		
};

class ThreadPool::Tasks
{
public:
	Tasks() : m_tasks(), m_isWorkToDo(false), m_mutex(new Mutex())  {}
	~Tasks(){delete m_mutex; m_mutex = NULL;}
	inline bool isWorkToDo() const {return m_isWorkToDo;}
   void lock() {m_mutex->enterCriticalSection();}
   void unlock() {m_mutex->exitCriticalSection();}
	ThreadTask* getNextTask()
	{
		//assert(!m_tasks.empty() && m_isWorkToDo);
      if(m_tasks.empty() || !m_isWorkToDo)
         return NULL;

		ThreadTask* task = *(m_tasks.begin());
		std::list<ThreadTask*>::iterator toDelete = m_tasks.begin();
		m_tasks.erase(toDelete);
		if(m_tasks.empty())
			m_isWorkToDo = false;

		return task;
	}
	void pushTask(ThreadTask* task)
	{		
		assert(task != NULL);
		m_tasks.push_back(task);
		m_isWorkToDo = true;
	}
private:
	std::list<ThreadTask* > m_tasks;
	bool m_isWorkToDo;
   Mutex* m_mutex;
	Tasks(const Tasks& C);
	const Tasks& operator=(const Tasks& C);		
};

ThreadPool::ThreadPool(CHUint numThreads) : 
	m_threads(new Threads()),
	m_tasks(new Tasks())
{
	m_workToDoEvent = CreateEvent(NULL,TRUE,FALSE,"WorkToDo");
	assert(m_workToDoEvent != NULL);
	for(CHUint i=0; i<numThreads; ++i)
		m_threads->m_threads.push_back(new WorkerThread(*this,m_workToDoEvent));
}
ThreadPool::~ThreadPool()
{
	killAllThreads();
	delete m_threads; m_threads = NULL;
	delete m_tasks; m_tasks = NULL;
}
void
ThreadPool::killAllThreads()
{
	CHUint numThreads = (CHUint)m_threads->m_threads.size();
	for(CHUint i=0; i<numThreads; ++i)
		m_threads->m_threads[i]->interrupt();	
}

void
ThreadPool::run()
{
   CHUint numthreads = (CHUint)m_threads->m_threads.size();
	for(CHUint i=0; i<numthreads; ++i)
		m_threads->m_threads[i]->execute();
}
void 
ThreadPool::addTask(ThreadTask* task)
{
	assert(task != NULL);
   m_tasks->lock();
	m_tasks->pushTask(task);
   m_tasks->unlock();

   BOOL result = SetEvent(m_workToDoEvent);
}

bool
ThreadPool::workToDo() const
{
   m_tasks->lock();
	bool moreWork = m_tasks->isWorkToDo();
   m_tasks->unlock();

	return moreWork;
}
bool
ThreadPool::threadsIdle() const
{
   CHUint numthreads = (CHUint)m_threads->m_threads.size();
	for(CHUint i=0; i<numthreads; ++i)
		if( m_threads->m_threads[i]->isWorking() )
         return false;

   return true;
}

void 
ThreadPool::finishTasks() const
{
	while( workToDo() || !threadsIdle() )
		Sleep(1000);
}		

ThreadTask*
ThreadPool::claimNextTask()
{
	Status stat = StatusFailure;

   m_tasks->lock();
   
   ThreadTask* task = m_tasks->getNextTask();
	if(!workToDo())
		ResetEvent(m_workToDoEvent);

   m_tasks->unlock();	
	return task;
}	
