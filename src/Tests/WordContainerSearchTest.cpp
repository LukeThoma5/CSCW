#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/word.h"
#include "../../headers/wordContainer.h"
#include "../../headers/badwordContainer.h"

using namespace std;

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
}

void createRandomWordWrongCounts();

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

bool checkWordContainerSorting(wordContainer& goodWords)
{
    createRandomWordWrongCounts(); //Make MSL scores random
    goodWords.sortWordContainer(); //Sort the list

    for (int i=0, GWSize=int(goodWords.size())-1; i<GWSize; ++i)
    {
        if (goodWords.at(i)->getwScore() < goodWords.at(i+1)->getwScore())
        {
            cout << "First item: " << goodWords.at(i)->getwScore() << "\nSecond item: " << goodWords.at(i+1)->getwScore() << endl;
            cerr << "Word Container Sorting Test [Failed]" << endl;
            return false;
        }
    }
    cout << "Word Container Sorting Test [Passed]" << endl;
    return true;
}

void runTestWordContainerStringCompare(wordContainer& goodWords)
{
	testWordContainerStringCompare(goodWords,0,goodWords.size()-2, 1,"WordContainerStringCompare Test Forward [");
	testWordContainerStringCompare(goodWords,goodWords.size()-1, 1, -1, "WordContainerStringCompare Test Backwards [");
}
