#include "../headers/logEvent.h"
#include <sstream>

using namespace std;

void split(const std::string &s,const char delim, std::vector<std::string>& elems);
std::vector<std::string> split(const std::string &s, const char delim='+');

logEvent::logEvent(const std::string& eventLine)
{
    vector<string> elems; //vector to store the event elements
    split(eventLine,'+',elems); //Split string into individual items
    eTime=stoul(elems[0],nullptr,16); //Set the time of the event equal to the int value after converting from hex
    eType=elems[1]; //Element 1 is the event type
    for (unsigned int i=2; i<elems.size(); i++) //Remaining items are data, add them to the dataItems vector
    {
        dataItems.push_back(elems[i]);
    }
    //cout << eTime << eType <<endl;
}

logEvent::logEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype)
{
    dataItems=dItems;
    eTime=etime;
    eType=etype;
}

time_t logEvent::getTime() const {return eTime;}

std::string logEvent::getType() const {return eType;}

std::vector<std::string> logEvent::getDataItems() const {return dataItems;}

string logEvent::generateEventLine()
{
    string retString = "";
    stringstream retStringStream(retString);
    retStringStream << std::hex << eTime << '+' << eType;
    for (int i=0; i<dataItems.size(); i++)
    {
        retStringStream << '+' << dataItems[i];
    }
    retStringStream.flush();
    return retStringStream.str();
}
