#include "hLog.h"
#include "logEvent.h"

using namespace std;

hLog::hLog(const std::string& saveLocation): logLocation(saveLocation)
{
    cout << "Creating log" << endl;
    ifstream logFile(saveLocation);
    string line; //string to store the current line that has been read in
    while (getline(logFile, line)) //getline takes an istream object and reads until a /n then stores the result in line. If no more lines to read returns false
    {
        cout << line << endl;
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
