#include "Tokenizer.h"
#include "string.h"

Tokenizer::Tokenizer(const char* buf, char delim) :
   m_buf(NULL),
   m_bufIter(NULL),
   m_delim(delim)
{
   size_t size = strlen(buf) +1; // + '\0'
   m_buf = new char[size];
   strcpy(m_buf, buf);

   m_bufIter = m_buf;
}

Tokenizer::~Tokenizer()
{
   if(m_buf)
      delete [] m_buf;
   m_buf = NULL;
}

const char* 
Tokenizer::getNext()
{
   if(*m_bufIter == '\0')
      return NULL;

   char* ret = m_bufIter;

   do
   {
      if( !(*ret) )
         ++ret;

      while(*m_bufIter && *m_bufIter != m_delim)
         ++m_bufIter;

      if(*m_bufIter != '\0')
      {
         *m_bufIter = '\0';
         ++m_bufIter;
      }
   }
   while( !(*ret) );

   return ret;
}