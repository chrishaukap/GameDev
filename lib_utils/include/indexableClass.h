#ifndef INDEXABLE_CLASS_H
#define INDEXABLE_CLASS_H

namespace CDH
{
#define IMPLEMENT_INDEXABLE_CLASS(SomeClass)\
   static int& GetClassIndexStatic()\
   {\
      static int index = -1;\
      return index;\
   }\
   virtual int& GetClassIndex()\
   {\
      /*assert(typeid(*this) == typeid(SomeClass));*/\
      return GetClassIndexStatic();\
   }
}
#endif //INDEXABLE_CLASS_H