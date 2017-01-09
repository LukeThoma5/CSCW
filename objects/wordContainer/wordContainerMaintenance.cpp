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

int wordContainer::findWordInsertionPoint(const string& searchWord)
{
    for (int i=0; i<wordList.size(); i++)
    {
        int compResult = stringCompare(searchWord,wordList[i]->getWord());
        cout << "Current word location " << wordList[i]->getWord() << " " << i  << " with compResult" << compResult << endl;
        if (compResult < 0)
        {
            cout << "Current word location " << wordList[i]->getWord() << " " << i  << endl;
            return i;
        }

    }
    return wordList.size(); //If it doesn't find a location, it must be the highest word so add to back.
}

void wordContainer::removeWord(int wordPosition)
{
    wordList.erase(wordList.begin()+wordPosition); //Remove the word* at position wordPosition. erase function takes a vector iterator which points at the word to be removed, begin function returns an iterator pointing to the start which is then incremented by wordPosition

    //Update wordPos to reflect the fact that a word has been removed
    for (int i=0; i<wordPos.size(); i++) //For every item in the list
    {
        if (wordPos[i] == wordPosition) //If it was the word that was erased, erase its position
            wordPos.erase(wordPos.begin()+i);
        if (wordPos[i] > wordPosition) //If the word was later in the list than the erased word, reduce the words index by 1
            wordPos[i]--;
    }
}

void wordContainer::deleteWord(int wordPosition)
{
    cout << "DELETING " << wordList[wordPosition]->getWord() << endl;
    delete wordList[wordPosition];
    removeWord(wordPosition);
}

int wordContainer::addWord(word* wordToAdd)
{
    //cout << "Trying to add word " << wordToAdd->getWord() << endl;
    //Needs to be improved/fixed
    //ADD the wordPos fixing!!
    int insertPos = findWordInsertionPoint(wordToAdd->getWord()); //Find the alphabetical location in the list to add the word to
    //cout << "Insertion position " << insertPos << endl;

    //wordList.push_back(wordToAdd);
    //Fix abstraction
    for (int i=0; i<wordPos.size(); i++) //for every word in the abstraction
    {
        if (wordPos[i] >= insertPos) //if the index for wordList held in the abstraction layer is greater than the insertionPoint the index in the abstraction layer needs to be incremented needs to be shifted to the right one.
            wordPos[i]++;
    }
    wordList.insert(wordList.begin()+insertPos,wordToAdd); //Add the word at the insertionPoint
    wordPos.push_back(insertPos); //Add the insertionPoint to the back of abstraction layer so everything else stays in order. Regardless of the words score it will be only acessible at the end until refreshwScores called.

    /* //Test code for printing the words around the new inserted word for manual inspection.
    for (int i=insertPos-1;i<insertPos+2;i++)
    {
        cout << i << ": " << wordList[i]->getWord() << endl;
    } */
    cout << "addWord Complete" << endl;

    int wordPosSize = wordPos.size(); //Convert to signed int so -1 doesn't cause an underflow when size is 0
    //return insertPos;
    return wordPosSize-1;
}