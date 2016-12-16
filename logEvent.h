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

	void delimItems(std::vector<std::string>& elems, const std::string& eventLine);
public:
	logEvent(const std::string& eventLine);

	logEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype);

	std::time_t getTime();

	std::string getType();

	std::vector<std::string> getDataItems();
	
};

#endif