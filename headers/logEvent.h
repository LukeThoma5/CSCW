#ifndef logEvent_h
#define logEvent_h
#include <iostream>
#include <string>
#include <vector>

#include <ctime>

class logEvent
{
	std::vector<std::string> dataItems;
	std::time_t eTime;
	std::string eType;
public:
	logEvent(const std::string& eventLine);

	logEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype);

	std::time_t getTime() const;

	std::string getType() const;

	std::vector<std::string> getDataItems() const;

	std::string generateEventLine();

};

#endif
