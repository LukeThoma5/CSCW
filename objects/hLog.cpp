#include "../headers/hLog.h"
#include "../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

//public
hLog::hLog(const std::string& saveLocation): logLocation(saveLocation)
{
    cout << "Creating log from file: " << logLocation << endl;
    ifstream logFile(logLocation);
    string line; //string to store the current line that has been read in
    while (getline(logFile, line,'\n')) //getline takes an istream object and reads until a /n then stores the result in line. If no more lines to read returns false
    {
        //cout << line << endl;
        log.push_back(logEvent(line));
    }
    logFile.close();
    cout << "Log created!" << endl;
    //Load in file to vector, possibly different event lists.
}

hLog::~hLog()
{
    int sessionLoc = findTimeStart(SSG::sessionStartTime);
    cout << log.size() - sessionLoc << " events to save to disk" << endl;
    if (log.size()-sessionLoc) //If number of items to save to disk is not 0
    {
        ofstream logFile(logLocation, std::ofstream::app); //Open file in append mode to save the new events;
        for (int i=sessionLoc; i<log.size(); i++)
        {
            logFile << log[i].generateEventLine() << endl;
        }
        logFile.close();
    }
}

void hLog::addEvent(const std::string& eventLine)
{
    log.push_back(logEvent(eventLine));
}

void hLog::addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype)
{
    log.push_back(logEvent(dItems,etime,etype));
}

void hLog::clearLog()
{
    cout << "Passwords MATCH, log clearing currently disabled!" << endl;
    /*
    ofstream logFile(logLocation, std::ofstream::out);
    logFile << endl;
    logFile.close();
    */
}
