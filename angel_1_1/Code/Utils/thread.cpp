#include "stdafx.h"
#include "thread.h"
#include <assert.h>

using namespace CDH;

Thread::Thread() :
   m_running(false),
   m_stop(false),
   m_threadID(0),
   m_handle(NULL)
{
}
Thread::~Thread()
{
   interrupt();
}

Status
Thread::execute()
{
   assert( !m_running );
   assert( m_threadID == 0 );

   if ( m_running ) return StatusFailure; // already running
   if ( m_threadID ) return StatusFailure; // already running

   m_stop = false;

   m_handle = ::CreateThread(NULL, 0, bootstrap, this, 0, &m_threadID);
   if(!m_handle) return StatusFailure; // create thread failed

   assert( m_threadID != 0 );

   //   block until the thread is actually running
   while ( !m_running && (m_threadID!=0) )
   {
      Sleep(10);
   }
   return StatusSuccess;
}

Status
Thread::interrupt()
{
   bool alreadyStopping = false;
   {
      if ( !m_running )
      {
         assert( m_threadID == 0 );
         return StatusSuccess;
      }
      assert( m_threadID != 0 );

      if ( m_stop )
         alreadyStopping = true;
      else
         m_stop = true;
   }
   
   if ( alreadyStopping )
   {
      while ( m_running )
      {
         Sleep(10);
      }
      assert( m_threadID == 0 );
      return StatusSuccess;
   }

   //   stop thread
   {
      DWORD exitCode = 0;
      if(!GetExitCodeThread(m_handle, &exitCode))
         return StatusFailure;

      while(exitCode == STILL_ACTIVE)
      {
         Sleep(10);

         if(!GetExitCodeThread(m_handle, &exitCode))
            return StatusFailure;
      }

      //   close thread handle
      CloseHandle(m_handle);
      m_handle = NULL;
   }

   assert(m_threadID==0);
   assert(!m_running);

   return StatusSuccess;
}

Thread::ThreadID
Thread::getCurrentThreadID()
{
   return GetCurrentThreadId();
}

Status
Thread::getTaskReturnValue() const
{
   assert(!m_running);
   return m_taskReturnValue;
}


unsigned long __stdcall
Thread::bootstrap(void *arg)
{
   Thread *argthread = (Thread *)arg;
     
   assert( !argthread->m_running );
   assert( argthread->m_threadID == getCurrentThreadID() );

   //   change state to running
   argthread->m_running = true;

   //   execute 
   Status err = StatusFailure;
   try
   {
      err = argthread->task();
   }
   catch(...)
   {
   }

   //   set return value, and change state to not-running
   argthread->m_taskReturnValue = err;
   argthread->m_running = false;
   argthread->m_threadID = 0;

   ExitThread(0);
   return 0;
}