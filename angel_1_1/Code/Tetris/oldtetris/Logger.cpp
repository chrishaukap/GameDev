#include "logger.h"

#include <iostream>
#include <fstream>
using namespace std;

Logger::Logger(void) :
   m_stream(0)
{
}

Logger::~Logger(void)
{
}

int Logger::open(LogLevel loglevel, const char* file)
{
   m_logLevel = loglevel;

   m_stream = new ofstream;
   m_stream->open( file );
   return 0;
}
int Logger::close()
{
   m_stream->close();
   return 0;
}
void Logger::writeLine(LogLevel loglevel, const char* line) const
{
   if( loglevel <= m_logLevel )
      *m_stream << line << endl;
}

void Logger::writeFormatLine(LogLevel loglevel, const char* fmt, va_list args) const
{
   if( loglevel > m_logLevel )
      return; // short circuit - we don't care about this log message

   int bufsiz = 1024;   
   const char *my_fmt = fmt;
   char *buf = new char[bufsiz];
   char *cbuf = buf;
   int usedLen = 0;

   usedLen = _vsnprintf(buf, bufsiz, my_fmt, args);
   if ((usedLen!= -1) && (usedLen+1 < bufsiz))     // length + \0
      *m_stream << buf << endl;
   else
      assert(!"_vsnprintf error.  should we put in a loop?");
}

void Logger::logErrorF( const char *fmt, ... ) const
{
   va_list args;
   va_start(args, fmt);
   writeFormatLine(LOG_ERROR, fmt, args);
   va_end(args);
}

void Logger::logDebugF( const char *fmt, ... ) const
{
   va_list args;
   va_start(args, fmt);
   writeFormatLine(LOG_DEBUG, fmt, args);
   va_end(args);
}

void Logger::logInfoF( const char *fmt, ... ) const
{
   va_list args;
   va_start(args, fmt);
   writeFormatLine(LOG_INFO, fmt, args);
   va_end(args);
}

void Logger::logWarningF( const char *fmt, ... ) const
{
   va_list args;
   va_start(args, fmt);
   writeFormatLine(LOG_WARNING, fmt, args);
   va_end(args);
}

