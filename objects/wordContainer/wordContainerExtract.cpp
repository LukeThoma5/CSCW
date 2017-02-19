#include <iostream>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"

using namespace std;

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
