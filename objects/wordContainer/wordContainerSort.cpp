#include <iostream>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
vector<int> splitVector(const vector<int>& inVector, int mode); //Now in main.cpp

void wordContainer::refreshwScores()
{
    for (int i=0, wordListSize=wordList.size(); i<wordListSize; ++i)
    {
        //For every word in the word list, regenerate its score
        wordList[i]->determineScore();
    }
}

void wordContainer::fillWordPos()
{
    wordPos.clear(); //Removes previous positions
    wordPos.reserve(wordList.size()); //Reduces the need to move the vector in memory by saying out the gate the miniumum space needed
    for (unsigned int i=0, wordListSize=wordList.size(); i<wordListSize; ++i)
    {
        wordPos.push_back(i); //Fill the word pos with sequential ints
    }
}

vector<int> wordContainer::containerMergeSort(const vector<int>& inVector)
{
    vector<int> leftVector = splitVector(inVector,0);  //Create left hand side (first half)
    vector<int> rightVector = splitVector(inVector,1); //Create right hand side (second half)

    int leftVectorSize = leftVector.size(); //Cache sizes to minimise the amount of function calls
    int rightVectorSize = rightVector.size();

    if (leftVectorSize != 1) //if left needs to be sorted
        leftVector = containerMergeSort(leftVector); //sort the left hand side
    if (rightVectorSize != 1)
        rightVector = containerMergeSort(rightVector);

    //Now assumed the left and right vectors are sorted
    int leftPos = 0; //Simple vector position pointers
    int rightPos = 0;

    vector<int> returnVector; //Create the return vector
    returnVector.reserve(inVector.size()); //Reduce the amount of mem allocs

    while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
    {
        if (leftPos == leftVectorSize) //If out of left hand values add a right
            returnVector.push_back(rightVector[rightPos++]); //add the next right value to the return list and increment rightPos counter
        else
        {
            if (rightPos == rightVectorSize) //If out of right hand values add a left
                returnVector.push_back(leftVector[leftPos++]);
            else
            {
                if (wordList[leftVector[leftPos]]->getwScore() >= wordList[rightVector[rightPos]]->getwScore())
                //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
                    returnVector.push_back(leftVector[leftPos++]); //Add the leftvector position
                else
                    returnVector.push_back(rightVector[rightPos++]);
            }
        }
    }

    return returnVector;
}

void wordContainer::sortWordContainer()
{
    cout << "Sorting wordContainer with " << wordList.size() << " words" << endl;
    refreshwScores(); //Recalculate all wScore values
    fillWordPos(); //Reset the wordPos so only sequential ints
    wordPos = containerMergeSort(wordPos); //Sort the ints based off the words they point to
    //Print the wordContainer values for manual debugging
    int stop = wordPos.size();
    if (stop > 10)
        stop = 10;
    printVector(wordPos,0,stop);
    for (unsigned int i=0; i<stop; i++)
    {
        cout << "Word: " << wordList[wordPos[i]]->getWord() << " at " << wordPos[i] << " with a value of " << wordList[wordPos[i]]->getwScore() << endl;
    }
}
