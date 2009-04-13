#ifndef CFGMANAGER_H
#define CFGMANAGER_H

#include "globals.h"

#include <iostream>
#include <fstream>
using namespace std;

// HAUKAP - need status codes

typedef unsigned short MyStatus;
const MyStatus Status_File_Not_Found = 1000;
const MyStatus Status_Invalid_Param = 1001;
const MyStatus Status_Unknown_CfgCommand = 1002;
const MyStatus Status_Success = 0;

class Logger;

class CFGManager
{
public:
   CFGManager(const Logger& logger);
   virtual ~CFGManager(void); 

   MyStatus init(const char* cfgFile);
   MyStatus parse();

   // accessors
   uint16 getMusicVolume() const {return m_musicVolume;}
   uint16 getEffectsVolume() const {return m_effectsVolume;}
   uint16 getStartingLevel() const {return m_startingLevel;}
   uint16 getFullScreen() const {return m_fullscreen;}
   RotDirection getRotDirection() const {return m_rotDirection;}
   RotDirection getRevRotDirection() const;

private:
   CFGManager();
   CFGManager(const CFGManager& C);
   const CFGManager& operator= (const CFGManager& C);
 
   ifstream* m_stream;
   const Logger& m_logger;
   
   uint16 m_musicVolume;
   uint16 m_effectsVolume;
   uint16 m_startingLevel;
   uint16 m_fullscreen;
   RotDirection m_rotDirection;
};

#endif // CFGMANAGER_H