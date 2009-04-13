#include "Tokenizer.h"
#include "threadPoolTest.h"
#include "threadTest.h"

using namespace CDH;

int main()
{
   ThreadTest test1;
   //test1.run();
   ThreadPoolTest test2;
   test2.run();

   /*basicThreadTest();
   {
      Tokenizer tok( "the cat ate" );
      const char* tmp1 = tok.getNext();
      const char* tmp2 = tok.getNext();
      const char* tmp3 = tok.getNext();
      const char* tmp4 = tok.getNext();
   }
   {
      Tokenizer tok( " the cat ate " );
      const char* tmp1 = tok.getNext();
      const char* tmp2 = tok.getNext();
      const char* tmp3 = tok.getNext();
      const char* tmp4 = tok.getNext();
   }*/
}