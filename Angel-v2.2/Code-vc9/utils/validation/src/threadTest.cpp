#include "threadTest.h"
using namespace CDH;

#include "thread.h"
#include <stdio.h>
#include <assert.h>


class Thread1 : public Thread
{
public:
   Thread1(int threadNum) :
      m_threadNum(threadNum)
   {}
   ~Thread1() {}

protected:
   Status task()
   {
      for(int i = 0; i < 25; i++)
      {
         fprintf(stderr, "threadNum = %d i = %d\n", m_threadNum, i);
         Sleep(m_threadNum * 10);
      }
      return StatusSuccess;
   }
private:
   int m_threadNum;
};

class SharedCounter : public Mutex
{
public:
	SharedCounter() : Mutex(), m_count(0)  {}
	void incrementCount()
	{
		++m_count;
	}
	CHUlong getCount() const
	{
		return m_count;
	}
private:
	CHUlong m_count;
};
class Thread2 : public Thread
{
public:
   Thread2(SharedCounter& counter, int threadNum) :
      m_counter(counter),
		m_threadNum(threadNum)
   {}
   ~Thread2() {}
protected:
   Status task()
   {
      for(int i = 0; i < 10; i++)
      {
			// comment out the "criticalsection()" functions and this will fail
			m_counter.enterCriticalSection();
			long preCounter = m_counter.getCount();
			Sleep( m_threadNum * 15 );

			m_counter.incrementCount();
			long postCounter = m_counter.getCount();

			assert(postCounter == (preCounter+1));		

         fprintf(stdout, "thread[%d] | count[%d]\n", m_threadNum, m_counter.getCount());
			m_counter.exitCriticalSection();
      }
      return StatusSuccess;
   }
private:
	SharedCounter& m_counter;
   int m_threadNum;
};



ThreadTest::ThreadTest(){}
ThreadTest::~ThreadTest(){}
void ThreadTest::mutexTest()
{
	fprintf(stderr, "threadMutexTest:\n");
	SharedCounter count;
   Thread2 t1(count,1);
   Thread2 t2(count,2);
   Thread2 t3(count,3);
   Thread2 t4(count,4);
   Thread2 t5(count,5);
   Thread2 t6(count,6);
   Thread2 t7(count,7);
   Thread2 t8(count,8);

   assert( t1.execute() == StatusSuccess);
   assert( t2.execute() == StatusSuccess);
   assert( t3.execute() == StatusSuccess);
   assert( t4.execute() == StatusSuccess);
   assert( t5.execute() == StatusSuccess);	
   assert( t6.execute() == StatusSuccess);	
   assert( t7.execute() == StatusSuccess);	
   assert( t8.execute() == StatusSuccess);	
}
void ThreadTest::threadTest()
{
   Thread1 t1(1);
   Thread1 t2(2);
   Thread1 t3(3);
   Thread1 t4(3);

   assert( t1.execute() == StatusSuccess);
   assert( t2.execute() == StatusSuccess);
   assert( t4.execute() == StatusSuccess);
   assert( t3.execute() == StatusSuccess);

   assert( t2.interrupt() == StatusSuccess);
   assert( t1.interrupt() == StatusSuccess);
   assert( t4.interrupt() == StatusSuccess);
   assert( t3.interrupt() == StatusSuccess);

   assert( t4.execute() == StatusSuccess);
   assert( t3.execute() == StatusSuccess);
   assert( t2.execute() == StatusSuccess);
   assert( t1.execute() == StatusSuccess);

   assert( t3.interrupt() == StatusSuccess);
   assert( t4.interrupt() == StatusSuccess);
   assert( t1.interrupt() == StatusSuccess);
   assert( t2.interrupt() == StatusSuccess);
}

void
ThreadTest::run()
{
   mutexTest();
   threadTest();
}
