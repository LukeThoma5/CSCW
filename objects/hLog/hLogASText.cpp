#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

void hLog::addGWW(string& retString, std::time_t startTime)
{
    vector<logEvent*> gww;
    getEventPointers(startTime,gww,"GoodWordWrong");
    retString +="\n";
    retString += to_string(gww.size());
    retString += " words have been wrong during this time:\n";
    for (int i=0; i<gww.size(); i++)
    {
        vector<string> eventData = gww[i]->getDataItems();
        retString += eventData[0];
        retString +=" -> ";
        retString +=eventData[2];
        retString += '\n';
    }
}

string hLog::getEventString(std::time_t startTime)
{
    cout << "Total events " << log.size() << "\nTotal items to display";
    int startLocation = findTimeStart(startTime);
    cout << int(log.size()) - startLocation << endl;
    string retString = generateWREStat(startTime);
    addGWW(retString, startTime);

    for (int i=startLocation; i<log.size(); i++)
    {
        int etime = log[i].getTime();
        int secondsSince = time(0) - etime;
        int daysSince = secondsSince / 86400;
        retString += to_string(daysSince) + " days ago " + log[i].getType() + '\n';
    }

    return retString;
}

int hLog::findTotalWREPlaytime(const std::vector<logEvent*>& events)
{
    int retInt = 0;
    for (unsigned int i=0; i<events.size(); i++)
    {
        retInt += stoi(events[i]->getDataItems()[0]);
    }
    return retInt;
}

string hLog::generateWREStat(time_t startTime)
{
    string statLine = "The user has spent a total of ";
    vector<logEvent*> events;
    getEventPointers(startTime,events,"HangmanComplete");
    statLine += to_string(findTotalWREPlaytime(events));
    statLine += " seconds playing over " + to_string(events.size()) + " play sessions.\n";
    return statLine;

}
