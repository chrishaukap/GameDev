#pragma once

#include <iostream>
#include <fstream>
using namespace std;

#include <stdio.h>
#include <stdarg.h>

#include <assert.h>

const char fileName[] = "log.txt";

enum LogLevel { NOLOG, LOG_ERROR, LOG_WARNING, LOG_INFO, 
                LOG_DEBUG = 10, LOG_DEBUG2 =20, LOG_DEBUG3 = 30, LOG_TRACE = 40 };

class Logger
{
public:
   Logger(void);
   ~Logger(void);

   int open( LogLevel logLevel = LOG_ERROR, const char* file = fileName );
   int close();

   void logTrace(const char* line) const { writeLine(LOG_TRACE,line);}
   void logTraceF( const char *fmt, ... ) const;
   void logDebug(const char* line) const { writeLine(LOG_DEBUG,line);}
   void logDebugF( const char *fmt, ... ) const;
   void logError(const char* line) const { writeLine(LOG_ERROR,line);}
   void logErrorF( const char *fmt, ... ) const;
   void logInfo(const char* line) const { writeLine(LOG_INFO,line);}
   void logInfoF( const char *fmt, ... ) const;
   void logWarning(const char* line)  const { writeLine(LOG_WARNING,line);}
   void logWarningF( const char *fmt, ... ) const;

   int seek() { assert(!"not implemented"); }

private:
   Logger( const Logger& L);
   const Logger& operator=(const Logger& L);
   
   void writeFormatLine(LogLevel loglevel, const char* fmt, va_list args) const;
   void writeLine( LogLevel log, const char* line ) const;

   ofstream* m_stream;
   LogLevel m_logLevel;
};
