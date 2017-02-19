#include <iostream>
#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"

using namespace std;

int wordContainer::findWordInsertionPoint(const string& searchWord)
{
    for (int i=0, end=wordList.size(); i<end; ++i)
    {
        //Run my string compare function due to special characters
        int compResult = stringCompare(searchWord,wordList[i]->getWord());
        cout << "Current word location " << wordList[i]->getWord() << " " << i  << " with compResult" << compResult << endl;
        if (compResult < 0) //If the first time the search word should be before, return current position
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
    delete wordList[wordPosition]; //Delete the object held by the pointer
    removeWord(wordPosition); //Remove the pointer and fix abstraction
}

int wordContainer::addWord(word* wordToAdd)
{
    //Find the alphabetical location in the list to add the word to
    int insertPos = findWordInsertionPoint(wordToAdd->getWord());

    //Fix abstraction
    for (int i=0; i<wordPos.size(); i++) //for every word in the abstraction
    {
        if (wordPos[i] >= insertPos) //if the index for wordList held in the abstraction layer is greater than the insertionPoint the index in the abstraction layer needs to be incremented needs to be shifted to the right one.
            wordPos[i]++;
    }
    wordList.insert(wordList.begin()+insertPos,wordToAdd); //Add the word at the insertionPoint
    wordPos.push_back(insertPos); //Add the insertionPoint to the back of abstraction layer so everything else stays in order. Regardless of the words score it will be only acessible at the end until refreshwScores called.

    cout << "addWord Complete" << endl;

    int wordPosSize = wordPos.size(); //Convert to signed int so -1 doesn't cause an underflow when size is 0

    return wordPosSize-1;
}
