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

badwordContainer::badwordContainer(): wordContainer()
{
    cout << "badwordContainer created!" << endl;
    containsBadWords = true;
}

badwordContainer::badwordContainer(wordContainer& fullWordList, string filename) //For use when building a bwordContainer from a file and the current known words
{
    containsBadWords = true;
    vector<string> wrongWordVector = loadDictFile(filename);
    if (!fullWordList.wordIndexValid) //If the index is not valid, generate the index
        fullWordList.generateWordIndex();
    unsigned int wrongWordVectorSize = wrongWordVector.size();
    for (unsigned int i=0; i<wrongWordVectorSize; i++)
    {
        string badWordWord = "";
        findWord(wrongWordVector[i],badWordWord);
        word* originalWord = fullWordList.wordList[fullWordList.findWordLocation(badWordWord)];
        //wordList.push_back(new badWord());
        cout << originalWord->wordC << endl;

        wordList.push_back(new badWord(originalWord, wrongWordVector[i]));
    }

    generateWordIndex();
}
