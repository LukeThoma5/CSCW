#include "../headers/hLog.h"
#include "../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern const std::time_t sessionStartTime;
}

//public
hLog::hLog(const std::string& saveLocation): logLocation(saveLocation) //Initialise the logLocation const variable, can not be changed after this
{
    cout << "Creating log from file: " << logLocation << endl;
    ifstream logFile(logLocation); //Open the log file for reading
    string line; //string to store the current line that has been read in
    while (getline(logFile, line,'\n')) //getline takes an istream object and reads until a /n then stores the result in line. If no more lines to read returns false
    {
        log.push_back(logEvent(line)); //Use the line to generate a logEvent and store it in a vector
    }
    logFile.close(); //Close the file
    cout << "Log created!" << endl;
}

hLog::~hLog()
//hLog destructor
{
    int sessionLoc = findTimeStart(SSG::sessionStartTime); //Find all the logs that were generated during the current program execution
    cout << log.size() - sessionLoc << " events to save to disk" << endl;
    if (log.size()-sessionLoc) //If number of items to save to disk is not 0
	//For loop wouldn't save anything to disk if no events where found but this if prevents needlessly opening and closing the file
    {
        ofstream logFile(logLocation, std::ofstream::app); //Open file in append mode to save the new events to the end
        for (int i=sessionLoc, logSize = log.size(); i<logSize; ++i)
		//For every event generated this runtime
        {
			//Save a line that could be used for construction to the logFile
            logFile << log[i].generateEventLine() << endl;
        }
        logFile.close(); //Close the logFile
    }
}

void hLog::addEvent(const std::string& eventLine)
{
	//Create a logEvent from the eventLine and add it to the log.
	//This function is never really used but is important to be part of the interface
    log.push_back(logEvent(eventLine));
}

void hLog::addEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype)
//Overloaded function which creates a log from varius data items
{
    log.push_back(logEvent(dItems,etime,etype));
}

void hLog::clearLog()
//Uncomment to enable logClearing, currently disabled to prevent accidental deletion
{
    cout << "Passwords MATCH, log clearing currently disabled!" << endl;
    /*
    ofstream logFile(logLocation, std::ofstream::out); //Open the file in overwrite mode
    logFile << endl; //Write a single empty line to the file (Empty the file)
    logFile.close(); //Close the file
    */
}

int hLog::size() const {return log.size();}
