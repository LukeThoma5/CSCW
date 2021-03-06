#ifndef hLog_h
#define hLog_h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "logEvent.h"

class hLog
{
protected:
	std::vector<logEvent> log;
	const std::string logLocation;

	std::string coordToString(const std::vector<float>& coords);

	void createScatterGraph(const std::string& graphName, const std::vector<float>& xcoords, const std::vector<float>& ycoords, const std::string& filename="dataToPlot.txt");

	void getEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType);

	void getWeekEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType);

	void addGWW(std::string& retString, std::time_t startTime);

	int findTotalWREPlaytime(const std::vector<logEvent*>& events);

	std::string generateWREStat(std::time_t startTime);

public:
	hLog(const std::string& saveLocation);

	~hLog();

	void addEvent(const std::string& eventLine);

	void addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype);

	int findTimeStart(std::time_t comparisonTime);

	void clearLog();

	std::string getEventString(std::time_t startTime=0);

	void graphIncorrectWords(std::time_t startPoint);

	void graphKeyboard(std::time_t startPoint, int eventItem, const std::string& graphName, const std::string& filename="dataToPlot.txt");

	void graphKeyboardMovingAvg(std::time_t startPoint, int eventItem, const std::string& graphName, const std::string& filename="dataToPlot.txt");

	void createBarGraph(const std::string& filename, const std::string& graphName, const std::vector<std::string>& xcoords, const std::vector<int>& ycoords);

	logEvent operator[](int i) const {return log[i];}

	int size() const;
};

#endif
