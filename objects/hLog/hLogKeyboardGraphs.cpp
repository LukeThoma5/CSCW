#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

void hLog::graphKeyboard(std::time_t startPoint, int eventItem, const string& graphName, const std::string& filename)
//Keyboard graph creator due to frequency of keyboard graphs
{
    vector<logEvent*> keyboardComplete; //Log event pointers container
    getEventPointers(startPoint, keyboardComplete, "keyboardComplete"); //Get keyboardComplete event pointers
    if (keyboardComplete.size()) //If atleast 1 event
    {
        vector<float> xcoords; //X values
        vector<float> ycoords; //Y values
        for (int i=0, keyboardCompleteSize=keyboardComplete.size(); i<keyboardCompleteSize; ++i)
        {
            xcoords.push_back(keyboardComplete[i]->getTime()); //Add the timestamp
            ycoords.push_back(stof(keyboardComplete[i]->getDataItems()[eventItem])); //Add the data item choosen by paramter
        }
        createScatterGraph(graphName,xcoords,ycoords,filename); //Pass the paramters to the createScatterGraph private function
    }
}

void hLog::graphKeyboardMovingAvg(std::time_t startPoint, int eventItem, const string& graphName, const std::string& filename)
//Most coments identical to above, changes commented
{
    vector<logEvent*> keyboardComplete;
    getEventPointers(startPoint, keyboardComplete, "keyboardComplete");
    if (keyboardComplete.size())
    {
        vector<float> xcoords;
        vector<float> ycoords;
        for (int i=0; i<keyboardComplete.size(); ++i)
		//For every event from the startPoint onwards
        {
            vector<logEvent*> itemsToAvg;
            getWeekEventPointers(keyboardComplete[i]->getTime(), itemsToAvg, "keyboardComplete"); //Get the pointers on a week around the current pointer
            float itemTotal = 0;
            for (int j=0; j<itemsToAvg.size(); j++) //For every pointer in the week to average
            {
                itemTotal += stof(itemsToAvg[j]->getDataItems()[eventItem]); //Add the dataItems to the total
            }

            xcoords.push_back(keyboardComplete[i]->getTime()); //Add the timestamp
            ycoords.push_back(itemTotal/float(itemsToAvg.size())); //Add the average of the pointer dataItems
        }
        createScatterGraph(graphName,xcoords,ycoords,filename); //Create the graph
    }
}
