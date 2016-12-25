#include "hLog.h"
#include "logEvent.h"
#include "SSG.h"
#include <cstdlib> //For system call

using namespace std;

//private
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

void hLog::createScatterGraph(const std::string& graphName, const std::vector<float>& xcoords, const std::vector<float>& ycoords)
{
    //Write the data to file to be loaded in by python script
    if (xcoords.size() == ycoords.size())
        if (xcoords.size() > 0)
        {
            ofstream graphData("dataToPlot.txt", std::ofstream::out);
            graphData << graphName << endl;
            //graphData << coordToString(xcoords) << endl << coordToString(ycoords);
            for (int i=0; i<xcoords.size(); i++)
            {
                graphData << to_string(xcoords[i]) << ',' << to_string(ycoords[i]) << endl;
            }
            graphData.close();
            //Call the python script
            system("python3 createGraph.py \"Scatter\"");
        }
        else
        {
            cout << "Cannot create an empty graph!" << endl;
        }
    else
        cout << "Coordinate mismatch, exiting" << endl;

}

//public
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
    int sessionLoc = findTimeStart(SSG::sessionStartTime);
    cout << log.size() - sessionLoc << " events to save to disk" << endl;
    if (log.size()-sessionLoc) //If number of items to save to disk is not 0
    {
        ofstream logFile(logLocation, std::ofstream::app); //Open file in append mode to save the new events;
        for (int i=sessionLoc; i<log.size(); i++)
        {
            //cout << log[i].getType() << endl;
            //cout << log[i].generateEventLine() << endl;
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

void hLog::graphIncorrectWords(std::time_t startPoint)
{
    vector<logEvent*> spellTestComplete;
    getEventPointers(startPoint,spellTestComplete,"SpellingTestComplete");
    if (spellTestComplete.size()) //If not 0
    {
        vector<float> xcoords;
        vector<float> ycoords;
        for (int i=0; i<spellTestComplete.size(); i++)
        {
            vector<string> dataItems = spellTestComplete[i]->getDataItems();
            float y1 = stof(dataItems[1]);
            float y2 = stof(dataItems[2]);
            double y = y1/y2;
            y=y*100;
            float x = spellTestComplete[i]->getTime();
            cout << "X: " << x << " Y: " << y << endl;
            xcoords.push_back(x);
            ycoords.push_back(y);
        }
        createScatterGraph("Percentage of words wrong against time",xcoords,ycoords);
    }
}

void hLog::graphKeyboard(std::time_t startPoint, int eventItem, const string& graphName)
{
    vector<logEvent*> keyboardComplete;
    getEventPointers(startPoint, keyboardComplete, "keyboardComplete");
    if (keyboardComplete.size())
    {
        vector<float> xcoords;
        vector<float> ycoords;
        for (int i=0; i<keyboardComplete.size(); i++)
        {
            vector<string> dataItems = keyboardComplete[i]->getDataItems();
            xcoords.push_back(keyboardComplete[i]->getTime());
            ycoords.push_back(stof(keyboardComplete[i]->getDataItems()[eventItem]));
        }
        createScatterGraph(graphName,xcoords,ycoords);
    }
}

int hLog::findTimeStart(std::time_t comparisonTime)
{
    // if (comparisonTime > log.back().getTime())
    //     return log.size();
    // for (int i=log.size(); i>=0; i--)
    // {
    //     if (log[i].getTime() < comparisonTime) //If current event was before the comp time, return the item past it
    //         return i+1;
    // }
    // return 0;
    //int lastValue=0;
    for (int i=0; i<log.size(); i++)
    {
        if (log[i].getTime() > comparisonTime)
            return i;
    }
    return log.size();
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
    string retString;
    addGWW(retString, startTime);

    for (int i=startLocation; i<log.size(); i++)
    {
        int etime = log[i].getTime();
        int secondsSince = time(0) - etime;
        int daysSince = secondsSince / 86400;
        retString += to_string(daysSince) + " days ago " + log[i].getType() + '\n';
    }

    SSG::MSL.sortList();
    retString += "The complete syllable list and wrong count:\n";
    for (int i=0; i<SSG::MSL.size(); i++)
    {
        retString += to_string(i+1) + ": " + SSG::MSL[i] + " with a wrong count of " + to_string(SSG::MSL.getSyllableWCount(i)) + '\n';
    }

    //graphKeyboard(startTime,2,"Mistakes per test");
    graphKeyboard(startTime,3,"Mistakes per 100 characters");
    //graphKeyboard(startTime,5,"WPM");

    return retString;
}
