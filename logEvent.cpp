#include "logEvent.h"
#include <sstream>

using namespace std;

void logEvent::delimItems(std::vector<std::string>& elems, const std::string& eventLine)
{
    stringstream ss(eventLine); //create a stringstream object from the string parameter as getline requires istream object
    string item; //string to hold the newly found item
    while (getline(ss, item, '+')) { //while still data left in the stringstream read until you encounter the delimiter, store the stream in the item string, execute loop, repeat
        if (!item.empty())
            elems.push_back(item);
    }
}

logEvent::logEvent(const std::string& eventLine)
{
    vector<string> elems;
    delimItems(elems,eventLine);
    dataItems=elems;
    eTime=time(0);
    eType="Undefined";
    cout << stoul(elems[0],nullptr,16) << endl;
}

logEvent::logEvent(const std::vector<std::string>& dItems,const std::time_t& etime,const std::string& etype)
{
    dataItems=dItems;
    eTime=etime;
    eType=etype;
}
