#include "../../headers/hLog.h"
#include "../../headers/logEvent.h"
#include <cstdlib> //For system call
#include <sstream>

using namespace std;

namespace SSG {
	extern std::time_t sessionStartTime;
}

void hLog::createBarGraph(const std::string& filename, const std::string& graphName, const std::vector<std::string>& xcoords, const std::vector<int>& ycoords)
{
    //Write the data to file to be loaded in by python script
    if (xcoords.size() == ycoords.size())
        if (xcoords.size() > 0)
        {
            ofstream graphData(filename, std::ofstream::out);
            graphData << graphName << endl;

            for (int i=0; i<xcoords.size(); i++)
            {
                graphData << xcoords[i] << ',' << to_string(ycoords[i]) << endl;
            }
            graphData.close();
            //Call the python script
            string command = "python3 createGraph.py \"Bar\" \"" + filename + "\"";
            system(command.c_str());
        }
        else
        {
            cout << "Cannot create an empty graph!" << endl;
        }
    else
        cout << "Coordinate mismatch, exiting" << endl;

}

void hLog::createScatterGraph(const std::string& graphName, const std::vector<float>& xcoords, const std::vector<float>& ycoords, const std::string& filename)
{
    //Write the data to file to be loaded in by python script
    if (xcoords.size() == ycoords.size())
        if (xcoords.size() > 0)
        {
            ofstream graphData(filename, std::ofstream::out);
            graphData << graphName << endl;
            //graphData << coordToString(xcoords) << endl << coordToString(ycoords);
            for (int i=0; i<xcoords.size(); i++)
            {
                graphData << to_string(xcoords[i]) << ',' << to_string(ycoords[i]) << endl;
            }
            graphData.close();
            //Call the python script
            string command = "python3 createGraph.py \"Scatter\" \"" + filename + "\"";
            system(command.c_str());
        }
        else
        {
            cout << "Cannot create an empty graph!" << endl;
        }
    else
        cout << "Coordinate mismatch, exiting" << endl;

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
        createScatterGraph("Percentage of words wrong against time",xcoords,ycoords,"./Data/graphData/wordsWrongAgainstTime.csv");
    }
}
