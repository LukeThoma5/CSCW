#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

std::string hLog::coordToString(const std::vector<float>& coords)
//Convert x or y lists to comma seperated values in a string
{
	//Only run if atleast 1 item
    string retString; //Make a string object
    stringstream retSS(retString); //Make a stringStream of that string object
    int lastFloat = coords.size()-1;
    for (int i=0; i<lastFloat; ++i) //For every item but the last
    {
        retSS << to_string(coords[i]) << ','; //Add the item and a comma
    }
    retSS << to_string(coords[lastFloat]); //Add the last item without a comma

    return retSS.str(); //Evaluate string stream back to string
}

int hLog::findTimeStart(std::time_t comparisonTime)
{
    for (int i=0, logEnd=log.size(); i<logEnd; ++i)
	//For every item in the log
    {
		//If the current event is later than the start time return this position
        if (log[i].getTime() > comparisonTime)
            return i;
    }
    return log.size(); //If none found return the end so no loops execute
}

void hLog::getEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType)
{
    int startLocation = findTimeStart(startPoint); //Find the index of the first item past the start
    for (int i=startLocation, logEnd=log.size(); i<logEnd; ++i)
	//For every item from the entry point to the end
    {
        if (log[i].getType() == eventType) //If the log type that we are searching for
            events.push_back(&log[i]); //Add its pointer to the events vector
    }
    cout << "Number of items found to plot " << events.size() << endl; //Debugging message
}

void hLog::getWeekEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType)
{
    time_t weekStart = startPoint - 302400; //startPoint - half a week
    time_t weekEnd = startPoint + 302400; //startPoint + half a week
    if (startPoint == 0) //If 0 set weekStart to 0 as it would have underflowed
        weekStart = 0;
    if (weekStart < 0) //If still somehow underflowed reset
        weekStart = 0;
    int startLocation = findTimeStart(weekStart); //Find index in vector from weekStart
    for (int i=startLocation, logSize=log.size(); i<logSize; ++i)
    {
        if (log[i].getTime() > weekEnd) //If past the week end
            break; //Stop adding the logEvent pointers

        if (log[i].getType() == eventType) //If of the type that we want
            events.push_back(&log[i]); //Add its pointer to the log
    }
    cout << "Number of items to average " << events.size() << endl; //Debug message
}
