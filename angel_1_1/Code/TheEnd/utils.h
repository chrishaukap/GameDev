#pragma once

#include "StringUtil.h"

struct NameValuePair
{
	NameValuePair(String thename, int thevalue) : name(thename), value(thevalue) {}
	String name;
	int value;
};