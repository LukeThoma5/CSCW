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
    //Get the item at the absolute index
    return wordPos[wordToGet]; //Return the value without any abstraction
}

word* wordContainer::at(int wordToGet)
{
    //This function gets the word from the wordPos list, eg sorted by wScore
    return wordList[wordPos[wordToGet]];
}

void wordContainer::printWordContainer() const
{
    //For every item in the container print its value and position
    for (int i=0, end=wordList.size(); i<end; ++i)
        cout << i << ": " << wordList[i]->getWord() << " ";
    cout << endl;
    //For every item in the abstraction, print its index
    for (int i=0, end=wordPos.size(); i<end; ++i)
        cout << i << ": " << wordPos[i] << " ";
    cout << endl;
}

void wordContainer::DisplaywScores(int start, int stop) const
{
    if (stop ==-1) //If defaulted value
        stop = wordList.size(); //Set the value to the end of the wordContainer
    for (unsigned int i=start; i<stop; ++i)
    {
        cout << wordList[i]->getwScore() << ",";
    }
    cout << endl;
}

int wordContainer::size() const
{
    return wordList.size();
}
