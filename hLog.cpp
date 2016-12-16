#include "hLog.h"
#include "logEvent.h"

using namespace std;

hLog::hLog(const std::string& saveLocation): logLocation(saveLocation)
{
    cout << "Creating log from file: " << logLocation << endl;
    ifstream logFile(logLocation);
    string line; //string to store the current line that has been read in
    while (getline(logFile, line,'\n')) //getline takes an istream object and reads until a /n then stores the result in line. If no more lines to read returns false
    {
        cout << line << endl;
        log.push_back(logEvent(line));
    }
    logFile.close();
    cout << "Log created!" << endl;
    //Load in file to vector, possibly different event lists.
}

hLog::~hLog()
{
    cout << "TODO add log save back to file!" << endl;
}

void hLog::addEvent(const std::string& eventLine)
{
    logEvent* myLog = new logEvent(eventLine);
    delete myLog;
}

void hLog::addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype)
{
    logEvent* myLog = new logEvent(dItems,etime,etype);
    delete myLog;
}

int hLog::findTimeStart(std::time_t comparisonTime)
{
    if (comparisonTime > log.back().getTime())
        return log.size();
    for (int i=log.size(); i>=0; i--)
    {
        if (log[i].getTime() < comparisonTime) //If current event was before the comp time, return the item past it
            return i+1;
    }
    return 0;
}

string hLog::getEventString(std::time_t startTime)
{
    string retString;
    for (int i=findTimeStart(startTime); i<log.size(); i++)
    {
        retString += to_string(log[i].getTime()) + log[i].getType() + '\n';
    }
    return retString;
}
