#pragma once

#include "cdhBase.h"
#include "thread.h"

namespace CDH
{
	class ThreadTask
	{
	public:
      ThreadTask(){}
		virtual ~ThreadTask(){}
		virtual Status doTask() =0;
	private:
		ThreadTask(const ThreadTask& C);
		const ThreadTask& operator=(const ThreadTask& C);		
	};

	class ThreadPool
	{
	public:
      friend class WorkerThread;
		ThreadPool(CHUint numThreads);
		~ThreadPool();

      void run();                      // go threads, go!
		void addTask(ThreadTask* task);	// adds task to queue

		// HAUKAP - should return status
		void finishTasks() const;			// blocks until threads are done with jobs
		void killAllThreads();				// terminates all threads

	private:
		ThreadTask* claimNextTask();		// HAUKAP - called by thread to get next job
		bool workToDo() const;				// returns true if their are queued tasks
      bool threadsIdle() const;        // returns false if any worker thread is still working

		HANDLE m_workToDoEvent;
		class Threads;
		Threads* m_threads;
		class Tasks;
		Tasks* m_tasks;

		ThreadPool();
		ThreadPool(const ThreadPool& C);
		const ThreadPool& operator=(const ThreadPool& C);		
	};
}