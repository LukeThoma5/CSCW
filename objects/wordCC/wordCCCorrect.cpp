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

void wordCC::wordCorrect(const int& wordPosition)
{
    //Add code to check if in good/bad container then call appropriate wordCorrect
    if (goodBadPos[wordPosition] == false)
    {
        cout << "goodWord wordCorrect will be called" << endl;
        goodWords.wordCorrect(wordPos[wordPosition]);
        printTop(0,10);
        //goodWords.DisplaywScores(0,10);
        //badWords.DisplaywScores(0,10);
    }
    if (goodBadPos[wordPosition] == true)
    {
        cout << "Badword wordCorrect will be called" << endl;
        badwordCorrect(wordPosition);
        //getWord(wordPosition)->wordWrong(attempt);
        //getWord(wordPosition)->determineScore();
        //printTop(0,10);
    }
}

void wordCC::badwordCorrect(const int& wordPosition)
{
    ;//wordContainer addword fixes its abstraction and places the location of the new word at the end of the wordPos vector Regardless of score or location in wordList. Adding word is fine, update the goodbadpos and set the value to the end of the wordContainer.
    //wordContainer remove word all the indexes are updated to point to new locations and the words index is removed from the wordPos vector and wordlist, all wordPos indexes after the word point need to be shifted one to the right. The underlying indexes are correct but the wordPos of wordCC needs updating.
    //badWord* toConvert = getWord(wordPosition);

    bool needsFixing = badWords.wordCorrect(wordPos[wordPosition]);

    if (needsFixing)
    {
        //Create data for logEvent;
        vector<string> dataItems;
        badWord* bwordToFix = badWords.getBadWord(wordPos[wordPosition]);
        dataItems.push_back(bwordToFix->getWord()); //Add the word
        vector<int> bwordSWC = bwordToFix->getSyllableWrongCount();
        for (unsigned int i=0; i<bwordSWC.size(); i++) //Add the syllableWrongCounts
            dataItems.push_back(to_string(bwordSWC[i]));
        SSG::histLog.addEvent(dataItems, time(0), "bwordToCorrect");

        //Move badword from badWords to a word in goodWords
        cout << "Score has fallen to 0! need to delete!" << endl;
        printwordCC();
        cout << "Word is bad and has value " << getWord(wordPosition)->getWord() << endl;
        word* turnedGoodWord = new word(getWord(wordPosition));
        cout << turnedGoodWord->getWord() << " has been created!" << endl;
        goodWords.addWord(turnedGoodWord); //Adds word to goodWords, has no major change to abs as it is added to the end.
        int goodWordSize = goodWords.size(); //cast to signed int
        cout << goodWords.at(goodWordSize-1)->getWord() << " at the end of goodWords" << endl;

        badWords.deleteWord(badWords.getABSIndex(wordPos[wordPosition])); //Delete the word at the index
        goodBadPos[wordPosition] = false; //Minor update the wordCC index
        wordPos[wordPosition] = goodWordSize-1;

        cout << "Word is now good and has value " << getWord(wordPosition)->getWord() << endl;

        //Fix abstraction
        for (int i=0; i<goodBadPos.size(); i++)
        {
            if (goodBadPos[i]==true) //if a bad word
                if (i>wordPosition) //if later in the list than the deleted word
                    wordPos[i]--; //Update the position
        }

        printwordCC();
    }
    else
    {
        cout << "word still got score > 0.0" << endl;
    }

}
