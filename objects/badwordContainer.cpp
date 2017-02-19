#include <fstream>

#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordContainer.h"
#include "../headers/badwordContainer.h"

using namespace std;

badwordContainer::badwordContainer(wordContainer& fullWordList, string filename) : saveLocation(filename) //Initialise constant value, can't change during runtime
//For use when building a bwordContainer from a file and the current known words
{
    vector<string> wrongWordVector = loadDictFile(filename); //Load the file into memory
    for (int i=0, wrongWordVectorSize=wrongWordVector.size(); i<wrongWordVectorSize; ++i)
    //for every line of the file
    {
        string badWordWord = ""; //Initialise an empty string
        findWord(wrongWordVector[i],badWordWord); //Find the badword from the line and sotre it in badWordWord string
        word* originalWord = fullWordList[fullWordList.findWordLocation(badWordWord)]; //Find the word pointer to the same word in the good word list

        wordList.push_back(new badWord(originalWord, wrongWordVector[i])); //Create a bad word from the good word using the badwordline from the file
        //Removing duplicates from the good and bad list is the job of the wordCC
    }
}

badWord* badwordContainer::at(int wordToGet)
//This function gets the word from the wordPos list, eg sorted by wScore
{
    word* wordToReturn = wordList[wordPos[wordToGet]]; //Locate the word pointer
    return static_cast<badWord*>(wordToReturn); //Cast the word* to a badword* since it is known bad
}

vector<string> badwordContainer::getBadWordList()
//Function used to get a list of all the words in the container so that they can be removed from the good word list
{
    vector<string> returnVector;
    //For every badword in the container
    for (int i=0, wordListSize=wordList.size(); i<wordListSize; ++i)
        returnVector.push_back(wordList[i]->getWord()); //Add its word string to the list
    return returnVector; //Return the list
}

badWord* badwordContainer::getBadWord(int wordToGet)
{
    if (wordToGet > -1) //Not an invalid index due to negative
    {
        if (wordToGet < wordList.size()) //If 0 to (container size-1)
        {
            if (wordList[wordToGet]->getWordFlags()[0] == 1) //If the word is a bad word, confirming before casting
                return static_cast<badWord*>(wordList[wordToGet]); //Return the badWord pointer from the stored word pointer
            else
                throw "wordContainer badWord* to word";
        }
        else
            throw "wordContainer Out of Bounds, int too large";
    }
    else
        throw "wordContainer Out of Bounds, negative!";
}

badwordContainer::~badwordContainer()
{
    ofstream badWordFile(saveLocation, std::ofstream::out); //Open file for writing (overwriting)
    for (int i=0, wordListSize=wordList.size(); i<wordListSize; ++i)
    {
        badWordFile << static_cast<badWord*>(wordList[i])->getBadWordLine() << endl; //Write the badWordLine to the file
    }
}
