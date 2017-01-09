#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

void hLog::graphKeyboard(std::time_t startPoint, int eventItem, const string& graphName, const std::string& filename)
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
        createScatterGraph(graphName,xcoords,ycoords,filename);
    }
}

void hLog::graphKeyboardMovingAvg(std::time_t startPoint, int eventItem, const string& graphName, const std::string& filename)
{
    vector<logEvent*> keyboardComplete;
    getEventPointers(startPoint, keyboardComplete, "keyboardComplete");
    if (keyboardComplete.size())
    {
        vector<float> xcoords;
        vector<float> ycoords;
        for (int i=0; i<keyboardComplete.size(); i++)
        {
            vector<logEvent*> itemsToAvg;
            getWeekEventPointers(keyboardComplete[i]->getTime(), itemsToAvg, "keyboardComplete");
            float itemTotal = 0;
            for (int j=0; j<itemsToAvg.size(); j++)
            {
                itemTotal += stof(itemsToAvg[j]->getDataItems()[eventItem]);
            }

            xcoords.push_back(keyboardComplete[i]->getTime());
            ycoords.push_back(itemTotal/float(itemsToAvg.size()));
        }
        createScatterGraph(graphName,xcoords,ycoords,filename);
    }
}
