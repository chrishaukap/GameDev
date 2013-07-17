#ifndef TOKENIZER_H
#define TOKENIZER_H

class Tokenizer
{
public:
   Tokenizer(const char* buf, char delim = ' ' );
   virtual ~Tokenizer();

   const char* getNext();

private:
   Tokenizer();
   Tokenizer( const Tokenizer& T );
   const Tokenizer& operator= (const Tokenizer& T);

   char* m_buf;
   char* m_bufIter;

   char m_delim;
};


#endif // TOKENIZER_H