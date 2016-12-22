#ifndef hLog_h
#define hLog_h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "logEvent.h"

class hLog
{
	std::vector<logEvent> log;
	const std::string logLocation;

	std::string coordToString(const std::vector<float>& coords);

	void createScatterGraph(const std::string& graphName, const std::vector<float>& xcoords, const std::vector<float>& ycoords);

	void getEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType);

public:
	hLog(const std::string& saveLocation);

	~hLog();

	void addEvent(const std::string& eventLine);

	void addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype);
	
	int findTimeStart(std::time_t comparisonTime);

	std::string getEventString(std::time_t startTime=0);

	void graphIncorrectWords(std::time_t startPoint);

};

#endif
