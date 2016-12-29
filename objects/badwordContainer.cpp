#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "word.h"
#include "badWord.h"
#include "wordContainer.h"
#include "badwordContainer.h"

using namespace std;

// badwordContainer::badwordContainer(): wordContainer()
// {
//     cout << "badwordContainer created!" << endl;
// }

badwordContainer::badwordContainer(wordContainer& fullWordList, string filename) : saveLocation(filename) //For use when building a bwordContainer from a file and the current known words
{
    vector<string> wrongWordVector = loadDictFile(filename);
    unsigned int wrongWordVectorSize = wrongWordVector.size();
    for (unsigned int i=0; i<wrongWordVectorSize; i++)
    {
        string badWordWord = "";
        findWord(wrongWordVector[i],badWordWord);
        word* originalWord = fullWordList[fullWordList.findWordLocation(badWordWord)];
        //wordList.push_back(new badWord());
        cout << originalWord->getWord() << endl;

        wordList.push_back(new badWord(originalWord, wrongWordVector[i]));
    }
}

badWord* badwordContainer::at(int wordToGet)
{
    //This function gets the word from the wordPos list, eg sorted by wScore
    word* wordToReturn = wordList[wordPos[wordToGet]];
    return static_cast<badWord*>(wordToReturn);
}

vector<string> badwordContainer::getBadWordList()
{
    vector<string> returnVector;
    for (unsigned int i=0; i<wordList.size(); i++)
    {
        returnVector.push_back(wordList[i]->getWord());
    }
    return returnVector;
}

badWord* badwordContainer::getBadWord(int wordToGet)
{
    if (wordToGet > -1)
    {
        if (wordToGet < wordList.size())
        {
            if (wordList[wordToGet]->getWordFlags()[0] == 1) //If the word is a bad word
                return static_cast<badWord*>(wordList[wordToGet]);
            else
                throw "wordContainer badWord* to word";
        }
        else
            throw "wordContainer Out of Bounds, int too large";
    }
    else
        throw "wordContainer Out of Bounds, negative!";

    //Add boundry checking and exception handling
    //Go back to fixing wordContainer::wordWrong
}

badwordContainer::~badwordContainer()
{
    ofstream badWordFile(saveLocation, std::ofstream::out); //Open file for writing
    for (unsigned int i=0; i<wordList.size(); i++)
    {
        badWordFile << static_cast<badWord*>(wordList[i])->getBadWordLine() << endl; //Write the badWordLine to the file
    }
}