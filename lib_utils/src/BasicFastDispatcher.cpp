#ifndef THREADS_H
#define THREADS_H

#include <Windows.h>

#include "cdhBase.h"

namespace CDH
{

	class Mutex
	{
	public:
		Mutex();
		virtual ~Mutex();
		Status enterCriticalSection();
		Status exitCriticalSection();

	private:
		HANDLE m_mutex; 

		Mutex(const Mutex& T);
		const Mutex& operator=(const Mutex& T);
	};

   class Thread
   {
   public:
      typedef DWORD ThreadID;

      Thread();
      virtual ~Thread();

      //calls task in new thread
      // return StatusSuccess if success, StatusFail otherwise
      Status execute();

      // causes shouldStop to return true and block until thread exits
      Status interrupt();

      inline ThreadID getThreadID() const  { return m_threadID; }
      inline bool isRunning() const { return m_running; }

      // call after isRunning() returns false
      Status getTaskReturnValue() const;

      // return current machine defined thread id
      static ThreadID getCurrentThreadID();

   protected:

      virtual Status task() = 0;

      // derived class's task() method calls this to see when to stop
      inline bool shouldStop() const { return m_stop; }

   private:

      static unsigned long __stdcall bootstrap(void *arg);

      bool m_running;
      bool m_stop;
      ThreadID m_threadID;
      Status m_taskReturnValue;

      HANDLE m_handle;

      Thread(const Thread& T);
      const Thread& operator=(const Thread& T);
   };
}

#endif
