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

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
void printVector(const std::vector<bool>& sV, int start=0, int stop=-1);
void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string seperateWord(const string& wordToSep);
string makeUpperCase(const string& attempt);

void wordCC::clearWordPos()
{
    wordPos.clear(); //Removes previous positions
    int totalSize = goodWords.size() + badWords.size();
    wordPos.reserve(totalSize); //Reduces the need to move the vector in memory by saying out the gate the miniumum space needed
    goodBadPos.clear();
    goodBadPos.reserve(totalSize);
    //for (unsigned int i=0; i<totalSize; i++)
    //{
    //    wordPos.push_back(0);
    //}
}

void wordCC::removeDuplicates(const vector<string>& dupWords)
{
    for (unsigned int i=0; i<dupWords.size(); i++)
    {
        //Search goodWords for this iterations badWord, this is important as the badWord is constructed from the goodword which must then be removed.
		int dupLocation;
		try {dupLocation = goodWords.binSearch(dupWords[i],0,goodWords.size());}
		catch (int errorNum)
		{
			dupLocation = -1;
			cout << "Failed to find word to delete, binSearch returned with code " << errorNum << endl;
		}
        cout << "Duplicate word " << dupWords[i] << " found in word container at location " << dupLocation << endl;
        //Delete the located word
        if (dupLocation != -1)
            goodWords.deleteWord(dupLocation);
    }
}

void wordCC::fixwordPos(const int& goodWordLocation)
{
    for (int i=0; i<wordPos.size(); i++)
    {
        if (goodBadPos[i] == false)
        {
            if (wordPos[i] > goodWordLocation)
                wordPos[i]--;
        }
    }
}

void wordCC::addWord(word* wordToAdd)
{
    //Add code to fix wordCC not wordContainer abstraction?
    goodWords.addWord(wordToAdd);
}
