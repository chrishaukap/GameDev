#ifndef BASICFAST_DISPATCHER_H
#define BASICFAST_DISPATCHER_H

#include "indexableClass.h"

#include <vector>
#include <assert.h>
template
<
   class BaseLhs,
   class BaseRhs = BaseLhs,
   typename ResultType = void,
   typename CallbackType = ResultType (*)(BaseLhs&, BaseRhs&)
>
class BasicFastDispatcher
{
   typedef std::vector<CallbackType> Row;
   typedef std::vector<Row> Matrix;
   Matrix m_callbacks;
   int m_nextIndex;
public:
   BasicFastDispatcher(): m_nextIndex(0) {}
   template <class SomeLhs, class SomeRhs>
   void Add(CallbackType pFun)
   {
      int& idxLhs = SomeLhs::GetClassIndexStatic();
      if( idxLhs<0 )
      {
         m_callbacks.resize(++m_nextIndex);
         idxLhs = (int)m_callbacks.size() - 1;
      }
      else if( (int)m_callbacks.size() <= idxLhs )
      {
         m_callbacks.resize(idxLhs + 1);
      }

      Row& thisRow = m_callbacks[idxLhs];
      int& idxRhs = SomeRhs::GetClassIndexStatic();
      if( idxRhs<0 )
      {
         thisRow.resize(++m_nextIndex);
         idxRhs = (int)thisRow.size() - 1;
      }
      else if( (int)thisRow.size() <= idxRhs )
      {
         thisRow.resize(idxRhs + 1);
      }
      thisRow[idxRhs] = pFun;
   }
   ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
   {
      int& idxLhs = lhs.GetClassIndex();
      int& idxRhs = rhs.GetClassIndex();
      if( idxLhs <0 || idxRhs <0 ||
          idxLhs >= (int)m_callbacks.size() ||
          idxRhs >= (int)m_callbacks[idxLhs].size() ||
          m_callbacks[idxLhs][idxRhs] == 0 )
      {
         fprintf(stderr, "No dispatch function found for this set.\n" );
			return;
      }
      return m_callbacks[idxLhs][idxRhs](lhs,rhs);
   }
};
#endif //BASICFAST_DISPATCHER_H