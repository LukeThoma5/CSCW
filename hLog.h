#ifndef hLog_h
#define hLog_h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "logEvent.h"

class hLog
{
	std::vector<std::string> log;
	const std::string logLocation; 
public:
	hLog(const std::string& saveLocation);
	
	~hLog();

	void addEvent(const std::string& eventLine);

	void addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype);
	
};

#endif
