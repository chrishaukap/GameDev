#include "credits.h"
#include <vector>
#include <assert.h>

#include "cdhBase.h"
using namespace CDH;

typedef std::vector<char*> TheCredits;
struct Credits::CreditsData
{
   CreditsData() : credits() {}
   ~CreditsData()
   {
      CHUint sz = (CHUint)credits.size();
      for( CHUint i=0; i<sz; ++i )
         delete credits[i];
      credits.clear();
   }
   TheCredits credits;
   TheCredits::const_iterator iter;
};

Credits::Credits() : 
   m_credits(new CreditsData())
{   
}
Credits::~Credits()
{
   delete m_credits;
}

void 
Credits::queueline(const char* line)
{
   assert(m_credits != NULL);
   if(line && *line)
      m_credits->credits.push_back( _strdup(line) );
}

const char* 
Credits::firstLine() const
{
   assert(m_credits != NULL);
   m_credits->iter = m_credits->credits.begin();
   return (*m_credits->iter);
}
const char* 
Credits::nextLine() const
{
   assert(m_credits != NULL);

   if(m_credits->iter == m_credits->credits.end())
      return NULL;
   ++(m_credits->iter);

   return (*m_credits->iter);   
}