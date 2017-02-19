#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

void hLog::addGWW(string& retString, std::time_t startTime)
//Function to add all of the first times a word was gotten wrong
{
    vector<logEvent*> gww;
    getEventPointers(startTime,gww,"GoodWordWrong"); //Get all of the times a good word has been wrong
    retString +="\n";
    retString += to_string(gww.size()); //Total number of times wrong passed in time
    retString += " words have been wrong during this time:\n";
    for (int i=0, gwwSize=gww.size(); i<gwwSize; ++i)
    {
        vector<string> eventData = gww[i]->getDataItems(); //Cache the log items
        retString += eventData[0]; //Add the word
        retString +=" -> "; // Add formatting
        retString +=eventData[2]; //Add what the user tried to spell it to
        retString += '\n';
    }
}

string hLog::getEventString(std::time_t startTime)
//Function to create the text for the Analysis screen
{
	//Total events and Total items on seperate lines but same pipe insertion
    cout << "Total events " << log.size() << "\nTotal items to display";
    int startLocation = findTimeStart(startTime); //Get where the events start after the passed in time (log.size if none in this time)
    cout << int(log.size()) - startLocation << endl; //Calculate the amount of events
    string retString = generateWREStat(startTime); //Initialise the retString to the time spent in word Related Entertainment
    addGWW(retString, startTime); //Add the words wrong to the return string

    for (int i=startLocation, logEnd=log.size(); i<logEnd; ++i)
    {
        int etime = log[i].getTime(); //Get the events timeStamp
        int secondsSince = time(0) - etime; //Find amount of seconds since event
        int daysSince = secondsSince / 86400; //Convert seconds to days (Int devision)
        retString += to_string(daysSince) + " days ago " + log[i].getType() + '\n'; //Add to return string
    }

    return retString; //Return retString to add to the TextView buffer
}

int hLog::findTotalWREPlaytime(const std::vector<logEvent*>& events)
{
    int retInt = 0;
    for (int i=0, eventsSize=events.size(); i<eventsSize; ++i)
    {
		//Add the time spent to the total, stoi = stringToInt
		//eventPointer->extractDataItems[The string holding the time in seconds]
        retInt += stoi(events[i]->getDataItems()[0]);
    }
    return retInt;
}

string hLog::generateWREStat(time_t startTime)
{
    string statLine = "The user has spent a total of ";
    vector<logEvent*> events;
    getEventPointers(startTime,events,"HangmanComplete"); //Gets the HangmanComplete events since time passed in
	statLine += to_string(findTotalWREPlaytime(events)); //Convert int to string and add to the statline
    statLine += " seconds playing over " + to_string(events.size()) + " play sessions.\n"; //Finish formating and add play session count that contributed
    return statLine;
}
