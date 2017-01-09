#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

std::string hLog::coordToString(const std::vector<float>& coords)
{
    string retString;
    stringstream retSS(retString);
    int lastFloat = coords.size()-1;
    for (int i=0; i<lastFloat; i++)
    {
        retSS << to_string(coords[i]) << ',';
    }
    retSS << to_string(coords[lastFloat]);

    return retSS.str();
}

int hLog::findTimeStart(std::time_t comparisonTime)
{
    for (int i=0; i<log.size(); i++)
    {
        if (log[i].getTime() > comparisonTime)
            return i;
    }
    return log.size();
}

void hLog::getEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType)
{
    int startLocation = findTimeStart(startPoint);
    for (int i=startLocation; i<log.size(); i++)
    {
        if (log[i].getType() == eventType)
            events.push_back(&log[i]);
    }
    cout << "Number of items found to plot " << events.size() << endl;
}

void hLog::getWeekEventPointers(std::time_t startPoint, std::vector<logEvent*>& events, const std::string& eventType)
{
    time_t weekStart = startPoint - 302400;
    time_t weekEnd = startPoint + 302400;
    if (startPoint == 0)
        weekStart = 0;
    if (weekStart < 0)
        weekStart = 0;
    int startLocation = findTimeStart(weekStart);
    for (int i=startLocation; i<log.size(); i++)
    {
        if (log[i].getTime() > weekEnd)
            break;

        if (log[i].getType() == eventType)
            events.push_back(&log[i]);
    }
    cout << "Number of items to average " << events.size() << endl;
}
