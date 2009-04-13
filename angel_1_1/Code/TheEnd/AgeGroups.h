#pragma once

#include "StringUtil.h"
#include <vector>

class AgeGroups
{
public:
   enum Group {Ages0to5, Ages5to10, Ages10to15, Ages15to20, Ages20to25, Ages25to30, 
               Ages30to35, Ages35to40, Ages40to45, Ages45to50, Ages50to55, Ages55to60, 
               Ages60to65, Ages65to70, Ages70to75, Ages75to80, Ages80to85, Ages85to90,
               InvalidAgeGroup};
   static String m_ageGroupStrings[]; 
private:
	AgeGroups();
};
