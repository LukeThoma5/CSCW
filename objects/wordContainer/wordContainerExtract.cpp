#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <math.h>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
vector<int> splitVector(const vector<int>& inVector, int mode); //Now in main.cpp

int wordContainer::getABSIndex(const int& wordToGet)
{
    return wordPos[wordToGet];
}

word* wordContainer::at(int wordToGet)
{
    //This function gets the word from the wordPos list, eg sorted by wScore
    return wordList[wordPos[wordToGet]];
}

void wordContainer::printWordContainer()
{
    for (int i=0; i<wordList.size(); i++)
        cout << i << ": " << wordList[i]->getWord() << " ";
    cout << endl;

    for (int i=0; i<wordPos.size(); i++)
    {
        cout << i << ": " << wordPos[i] << " ";
    }
    cout << endl;
}

void wordContainer::DisplaywScores(int start, int stop)
{
    if (stop ==-1)
        stop = wordList.size();
    for (unsigned int i=start; i<stop; i++)
    {
        cout << wordList[i]->getwScore() << ",";
    }
    cout << endl;
}

int wordContainer::size() const
{
    return wordList.size();
}
