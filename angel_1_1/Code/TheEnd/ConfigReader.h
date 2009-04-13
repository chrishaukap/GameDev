#pragma once


class ConfigReader
{
public:
	static void readConfigFile(const char* cfg);
	

private:
	ConfigReader();
	static bool done;
};