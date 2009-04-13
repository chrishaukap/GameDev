#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <vector>

#include "ConfigReader.h"
#include "LifeTrophies.h"
#include "AgeGroups.h"

static const int MAX_STRING_LENGTH = 1000;
static char g_buf[MAX_STRING_LENGTH];

static void 
Tokenize(const std::string& str,
         std::vector<std::string>& tokens,
         const std::string& delimiters = " ")
{
   std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

   while (std::string::npos != pos || std::string::npos != lastPos)
   {
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      lastPos = str.find_first_not_of(delimiters, pos);
      pos = str.find_first_of(delimiters, lastPos);
   }
}

static void parseTrophy(FILE* file, const char* name)
{
	LifeTrophy trophy;
	trophy.m_name = name;

	char *tok1=NULL, *tok2=NULL;
	char *context;
	do
	{
		fgets(g_buf, MAX_STRING_LENGTH, file);

		tok1 = strtok_s(g_buf, ":\n", &context);
		tok2 = strtok_s(NULL, ":\n", &context);
		if( 0== _stricmp(tok1, "description") )
			trophy.m_description = _strdup(tok2);
		else if( 0== _stricmp(tok1, "ages") )
		{
			const char* minage = strtok_s(tok2, "-\n", &context);
			const char* maxage = strtok_s(NULL, "-\n", &context);
			trophy.m_minAgeGroup = (AgeGroups::Group)(atoi(minage)/5);
			trophy.m_maxAgeGroup = (AgeGroups::Group)(atoi(maxage)/5);
		}
		else if( 0== _stricmp(tok1, "choiceRequirements") )
		{
         std::vector<std::string> nameValPairs;
         Tokenize(tok2, nameValPairs, ",");

         unsigned int sz = (unsigned int)nameValPairs.size();
         for(unsigned int i=0; i<sz; ++i )
         {
            char* name = _strdup(nameValPairs[i].c_str());
            char* value = strstr(name, "=");
            *value = '\0';
            ++value;
            trophy.m_choiceRequirements[String(name)] = atoi(value);
            delete name;
         }
		}
		else if( 0== _stricmp(tok1, "Upkeep") )
		{
         std::vector<std::string> nameValPairs;
         Tokenize(tok2, nameValPairs, ",");

         unsigned int sz = (unsigned int)nameValPairs.size();
         for(unsigned int i=0; i<sz; ++i )
         {
            char* name = _strdup(nameValPairs[i].c_str());
            char* value = strstr(name, "=");
            *value = '\0';
            ++value;
            trophy.m_upkeep.push_back( NameValuePair(String(name), atoi(value)) );
            delete name;
         }
		}
		else if( 0== _stricmp(tok1, "TrophyRequirements") )
		{
			Tokenize(tok2, trophy.m_trophyRequirements, ",");
		}
	}
	while( 0!= _stricmp(tok1,"END_TROPHY") );
	theLifeTrophies.pushBack(trophy);
}

static void parseChoice(FILE* file, const char* name)
{
	char *tok1=NULL, *tok2=NULL;
	char *context;
	do
	{
		fgets(g_buf, MAX_STRING_LENGTH, file);

		tok1 = strtok_s(g_buf, ":\n", &context);
		tok2 = strtok_s(NULL, ":\n", &context);
	}
	while( 0!= _stricmp(tok1,"END_CHOICE") );
}
bool ConfigReader::done = false;

void
ConfigReader::readConfigFile(const char* cfg)
{
	FILE *file = fopen(cfg, "r");
	if (file != NULL)
	{
		while (!feof(file))
		{
			fgets(g_buf, MAX_STRING_LENGTH, file);

			char *context;
			char *tok1 = strtok_s(g_buf, ":\n", &context);
			char *tok2 = strtok_s(NULL, ":\n", &context);
			if(tok1 != NULL)
			{
				if( 0== _stricmp(tok1, "BEGIN_TROPHY") )
				{
					parseTrophy(file,tok2);
				}
				else if( 0== _stricmp(tok1, "BEGIN_CHOICE") )
				{
				
				}
			}
		}
		fclose(file);
	}


}