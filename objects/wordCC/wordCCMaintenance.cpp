#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <ctime>

#include "../../headers/wordContainer.h"
#include "../../headers/badwordContainer.h"
#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"
#include "../../headers/randng.h"

namespace SSG {
	extern hLog histLog;
}

using namespace std;

void wordCC::clearWordPos()
{
    wordPos.clear(); //Removes previous positions
    int totalSize = goodWords.size() + badWords.size();
    wordPos.reserve(totalSize); //Reduces the need to move the vector in memory by saying out the gate the miniumum space needed
    goodBadPos.clear(); //Remove good/bad indicators
    goodBadPos.reserve(totalSize); //Reduce mem allocs
}

void wordCC::removeDuplicates(const vector<string>& dupWords)
{
    for (unsigned int i=0; i<dupWords.size(); i++)
    {
        //Search goodWords for this iterations badWord, this is important as the badWord is constructed from the goodword which must then be removed.
		int dupLocation;
		try {dupLocation = goodWords.binSearch(dupWords[i],0,goodWords.size());}
		catch (int errorNum)
		//If the word could not be found in the goodWords container, it is not duplicated. Don't try and remove again
		{
			dupLocation = -1;
			cout << "Failed to find word to delete, binSearch returned with code " << errorNum << endl;
		}
        cout << "Duplicate word " << dupWords[i] << " found in word container at location " << dupLocation << endl;
        //Delete the located word
        if (dupLocation != -1) //If word found
            goodWords.deleteWord(dupLocation); //Tell goodWords to destruct word and fix its abstraction
    }
}

void wordCC::fixwordPos(const int& goodWordLocation)
{
    for (int i=0, end=wordPos.size(); i<end; ++i)
    {
        if (goodBadPos[i] == false)
		//If a good word
        {
            if (wordPos[i] > goodWordLocation) //If later in the alphabet than the word that was removed
                wordPos[i]--; //Shift it back by one
        }
    }
}

void wordCC::addWord(word* wordToAdd)
{
    goodWords.addWord(wordToAdd);
	//The word is added in the alphabetical order in the goodWords list but at the end of its abstraction so that is unaffected. The word is inassessible to the outside until a call is made to findHardest (findSpellingWords or findKeyboardWords)
}
