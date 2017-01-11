#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include <gtkmm.h>

#include "../../headers/randng.h" //random number generator header
#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/masterSyllableListTree.h"
#include "../../headers/masterSyllableListMap.h"
#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"
#include "../../headers/badwordContainer.h"
#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

#include "../../objects/windowContainer.cpp"

using namespace std;
#include "MSLUtils.h"

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	//masterSyllablesListTree physicalMSL;
    extern masterSyllablesListMap physicalMSL;
	extern masterSyllablesList* MSL; //MasterSyllablesList
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern hLog histLog;
	extern time_t sessionStartTime;
}

void createRandomWordWrongCounts();
void printVector(const vector<string>& sV, int start=0, int stop=-1);
void printVector(const vector<int>& sV, int start=0, int stop=-1);

bool testWordContainerSearch(wordContainer& goodWords)
{
    //string.compare has issues when passed "antic" and "anti-Federalist", doesn't like the -
    //bool success = true;
    int j = 0;
    for (int i=0; i<goodWords.size(); i++)
    {
        //cout << "### Now searching for " << goodWords[i]->getWord() << endl;
        int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
        if (location == -1)
        {
            cout << "Word not found" << endl;
            return false;
        }
        if (i != location)
        {
            cout << "Binary Search Test [Failed]" << endl;
            cout << "An error has occured " << i << " != " << location << endl;
            return false;
        }
        j++;
        if (j > 1000)
        {
            int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
            //cout << "Looking for word # " << i << " with value " << goodWords[i]->getWord() << " which was found at location " << location << " with a value of " << goodWords[location]->getWord() << endl;
            j=0;
        }
    }
    cout << "Binary Search Test [Passed]" << endl;
    return true;
}

bool getConditionWCSC(int end, const int& direction, const int i)
{
	if (direction == -1)
		return (i > end);
	return (i<end);
}

bool testWordContainerStringCompare(wordContainer& goodWords, int start, int end,const int direction, const string& testString)
{
	for (int i=start; getConditionWCSC(end,direction,i); i+=direction)
	{
		if (goodWords.stringCompare(goodWords[i]->getWord(), goodWords[i+direction]->getWord()) == direction)
		{
			cout << "Test failing on item " << i << " comparing " << goodWords[i]->getWord() << " to " << goodWords[i+direction]->getWord() << endl;
			cout << testString <<  "Failed]" << endl;
			return false;
		}
	}
	cout << testString << "Passed]" << endl;
	return true;
}

void runTestWordContainerStringCompare(wordContainer& goodWords)
{
	testWordContainerStringCompare(goodWords,0,goodWords.size()-2, 1,"WordContainerStringCompare Test Forward [");
	testWordContainerStringCompare(goodWords,goodWords.size()-1, 1, -1, "WordContainerStringCompare Test Backwards [");
}
