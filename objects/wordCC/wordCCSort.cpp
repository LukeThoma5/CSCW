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

void wordCC::generatewScore()
{
    cout << "Determining Scores" << endl;

    //goodWords[8122]->determineScore();
    goodWords.sortWordContainer();
    badWords.sortWordContainer();

    //goodWords.DisplaywScores(100,120);

    badWords.DisplaywScores();

}

void wordCC::findHardest()
{
    clearWordPos();
    combineWordLists();
    currentWord=0;
}

void wordCC::combineWordLists()
{
    //This assumes that generatewScore has already been called so the two lists are already sorted themselves.
    int leftPos = 0; //Simple vector position pointers
    int rightPos = 0;

    int leftVectorSize = goodWords.size();
    int rightVectorSize = badWords.size();

    while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
    {
        if (leftPos == leftVectorSize) //If out of left hand values add a right
        {
            wordPos.push_back(rightPos++); //add the next right value to the return list and increment rightPos counter
            goodBadPos.push_back(true);
        }
        else
        {
            if (rightPos == rightVectorSize) //If out of right hand values add a left
            {
                //cout << "Adding word " << leftPos << true;
                wordPos.push_back(leftPos++);
                goodBadPos.push_back(false);
            }

            else
            {
                if (goodWords.at(leftPos)->getwScore() >= badWords.at(rightPos)->getwScore()) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
                {
                    //cout << "Adding word " << leftPos << true;
                    wordPos.push_back(leftPos++); //Add the leftvector position
                    goodBadPos.push_back(false);
                }
                else
                {
                    wordPos.push_back(rightPos++);
                    goodBadPos.push_back(true);
                }
            }
        }
    }

    printVector(wordPos,0,10);
    printVector(goodBadPos,0,10);

    //cout << "The difference in locations is " << wordPos.size()-goodBadPos.size() << endl;
}
