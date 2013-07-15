#ifndef THREADTEST_H
#define THREADTEST_H

namespace CDH
{

   class ThreadTest
   {
   public:
      ThreadTest();
      ~ThreadTest();
      void mutexTest();
      void threadTest();
      void run();
   };
}
#endif // THREADTEST_H