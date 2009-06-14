#include "cfgmanager.h"
#include "string.h"
#include "Tokenizer.h"
#include "globals.h"

#include <iostream>
#include <fstream>
using namespace std;

CFGManager::CFGManager(const Logger& logger) :
   m_stream(NULL),
   m_logger(logger),
   m_musicVolume(0),
   m_effectsVolume(0),
   m_startingLevel(0),
   m_fullscreen(FULLSCREEN),
   m_rotDirection(CW)
{
}

CFGManager::~CFGManager(void)
{
   m_stream->close();
}

MyStatus 
CFGManager::init(const char* cfgFile)
{
   if( !cfgFile )
      return Status_Invalid_Param;

   MyStatus sts = Status_Success;
   m_logger.logInfoF( "Opening Config File: %s", cfgFile );
   
   // HAUKAP - should test for existence of file.
   m_stream = new ifstream;
   m_stream->open( cfgFile );

   return sts;
}

MyStatus 
CFGManager::parse()
{
   MyStatus sts = Status_Success;
   const uint16 BUF_SIZE = 10000;

   char cfgLine[BUF_SIZE] = {0}; // HAUKAP - static sized buf
   while( !m_stream->eof() )
   {
      m_stream->getline( cfgLine, BUF_SIZE );

      // ignore comments
      {
         char* tmp = cfgLine;
         if( *tmp == '#' )
            continue;
         while( *tmp++ == ' ' )
            ;
      }
      
      Tokenizer tok(cfgLine);

      const char* cfgOption = tok.getNext();
      if( !cfgOption )
         continue;

      char buf[256];

      // HAUKAP - should define the constant strings elsewhere
      if( 0== _stricmp(cfgOption, "MusicVolume") )
      {
         const char* musicVolume = tok.getNext();
         int value = atoi(musicVolume);
         if( value < MIN_MUSIC_VOLUME || value > MAX_MUSIC_VOLUME )
         {
            m_logger.logWarningF( "Invalid cfg value for MusicVolume: %d." );
            m_musicVolume = static_cast<uint16>(MIN_MUSIC_VOLUME + (MAX_MUSIC_VOLUME - MIN_MUSIC_VOLUME)/2);
         }
         else
         {
            m_musicVolume = static_cast<uint16>(value);
            m_logger.logInfoF( "Setting MusicVolume CFG option to %d", m_musicVolume );
         }
      }
      else if(0== _stricmp( cfgOption, "EffectsVolume") )
      {
         const char* effectsVolume = tok.getNext();
         int value = atoi(effectsVolume);
         if( value < MIN_EFFECTS_VOLUME || value > MAX_EFFECTS_VOLUME )
         {
            m_logger.logWarningF( "Invalid cfg value for EffectsVolume: %d." );
            m_effectsVolume = static_cast<uint16>(MIN_EFFECTS_VOLUME + (MAX_EFFECTS_VOLUME - MIN_EFFECTS_VOLUME)/2);
         }
         else
         {
            m_effectsVolume = static_cast<uint16>(value);
            m_logger.logInfoF( "Setting EffectsVolume CFG option to %d", m_effectsVolume );
         }
      }
      else if(0== _stricmp( cfgOption, "StartingLevel") )
      {
         const char* startingLevel = tok.getNext();
         int value = atoi(startingLevel);
         if( value < MIN_LEVEL || value > MAX_LEVEL )
         {
            m_logger.logWarningF( "Invalid cfg value for StartingLevel: %d.  Defaulting to MIN_LEVEL" );
            m_startingLevel = MIN_LEVEL;
         }
         else
         {
            m_startingLevel = static_cast<uint16>(value);
            m_logger.logInfoF( "Setting StartLevel CFG option to %d", m_startingLevel );
         }
      }
      else if(0== _stricmp( cfgOption, "RotationDirection") )
      {
         // HAUKAP - should define these constant strings elsewhere.  they're used elsewhere.
         const char* rotationDirection = tok.getNext();
         if( rotationDirection && 0== _stricmp( rotationDirection, "ClockWise" ) )
         {
            m_rotDirection = CW;
            m_logger.logInfoF( "Setting RotationDirection CFG option to ClockWise" );
         }
         else if( rotationDirection && 0== _stricmp( rotationDirection, "CounterClockWise" ) )
         {
            m_rotDirection = CCW;
            m_logger.logInfoF( "Setting RotationDirection CFG option to CounterClockWise" );
         }
         else
         {            
            m_logger.logWarningF( "Invalid cfg value for RotationDirection: %s.  Defaulting to ClockWise", 
                                  rotationDirection );
            m_rotDirection = CW;
         }
      }
      else if(0== _stricmp( cfgOption, "FullScreen") )
      {
         const char* fullScreen = tok.getNext();
         if( fullScreen && 0== _stricmp( fullScreen, "TRUE" ) )
         {
            m_logger.logInfoF( "Running in fullscreen mode" );
            m_fullscreen = FULLSCREEN;
         }
         else if( fullScreen && 0== _stricmp( fullScreen, "FALSE" ) )
         {
            m_logger.logInfoF( "Running in windowed mode" );
            m_fullscreen = WINDOWED;    
         }
         else
         {
            m_fullscreen = WINDOWED;  
            m_logger.logWarningF( "Invalid cfg value for FullScreen: %s.  Defaulting to FALSE", 
                                  fullScreen );  
            m_logger.logInfoF( "Running in windowed mode" );
         }
      }
      else
      {
         m_logger.logWarningF( "Ingoring unknown Config Command: %s", buf );
         sts = Status_Unknown_CfgCommand;
      }
   }

   return sts;

}

RotDirection 
CFGManager::getRevRotDirection() const 
{
   switch(m_rotDirection)
   {
   case CW: return CCW;
   case CCW: return CW;
   default: 
      assert( !"Unknown Rotation Direction.  Code should NOT reach this point" );
      return CW;
   }
}