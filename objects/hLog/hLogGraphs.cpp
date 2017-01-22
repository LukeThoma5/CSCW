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
    if (xcoords.size() == ycoords.size()) //If programmer not made a mistake
        if (xcoords.size() > 0) //If the amount of items is more than none, eg something to plot
        {
            ofstream graphData(filename, std::ofstream::out); //Open the passed in filename
            graphData << graphName << endl; //Write the title to the first line

            for (int i=0, xcoordSize=xcoords.size(); i<xcoordSize; ++i)
			//For every named item
            {
				//Write the item name demlimited by a comma then the amount of times it occurs then start a new line
                graphData << xcoords[i] << ',' << to_string(ycoords[i]) << endl;
            }
            graphData.close(); //Close the file so the python script can read it
            //Call the python script
			//Order the command to create graph with the arguments "Bar" and "path to file"
            string command = "python3 createGraph.py \"Bar\" \"" + filename + "\"";
            system(command.c_str()); //Run the command
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
    if (xcoords.size() == ycoords.size()) //If all coordinates complete
        if (xcoords.size() > 0) //If at least one coordinate to plot
        {
            ofstream graphData(filename, std::ofstream::out); //Open the file passed in using overwrite mode
            graphData << graphName << endl; //Write the title to the first line

            for (int i=0, xcoordSize=xcoords.size(); i<xcoordSize; ++i)
            {
				//For every coordinate save its x and y demlimited by a , on a new line
                graphData << to_string(xcoords[i]) << ',' << to_string(ycoords[i]) << endl;
            }
            graphData.close(); //Close the file so the python script can read it
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
    vector<logEvent*> spellTestComplete; //Vector to be filled with pointers by next line
    getEventPointers(startPoint,spellTestComplete,"SpellingTestComplete"); //Get all the pointers to SpellingTestComplete from the startTime onwards and place them in the spellTestComplete vector
    if (spellTestComplete.size()) //If at least one point to graph
    {
		//Create the vectors to store the coordinates
        vector<float> xcoords;
        vector<float> ycoords;

        for (int i=0, spellTestCompleteSize=spellTestComplete.size(); i<spellTestCompleteSize; ++i)
		//For every SpellingTestComplete logEvent from the startPoint onwards
        {
            vector<string> dataItems = spellTestComplete[i]->getDataItems(); //Get a copy of the dataItems
			//stof stringToFloat convertion
            float y1 = stof(dataItems[1]); //Number of incorrect words
            float y2 = stof(dataItems[2]); //Total number of words tested
            double y = y1/y2;
            y=y*100; //Create a percentage of words incorrect
            float x = spellTestComplete[i]->getTime(); //Get the timestamp and convert to a float to match interface
            cout << "X: " << x << " Y: " << y << endl;
            xcoords.push_back(x); //Add the coordinates
            ycoords.push_back(y);
        }
		//Create the scatter graph with this title in the Data/graphData subdirectory
        createScatterGraph("Percentage of words wrong against time",xcoords,ycoords,"./Data/graphData/wordsWrongAgainstTime.csv");
    }
}
