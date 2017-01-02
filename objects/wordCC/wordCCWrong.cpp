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

void wordCC::wordWrong(const int& wordPosition,const string& attempt)
{
    spellingWrongWordCount++; //Increase the number of incorrect words this run
    if (goodBadPos[wordPosition] == true)
    {
        cout << "badWord wordWrong will be called" << endl;
        getWord(wordPosition)->wordWrong(attempt);
        getWord(wordPosition)->determineScore();
        printTop(0,10);
        cout << "The goodwords list" << endl;
        //goodWords.DisplaywScores(0,10);
        //badWords.DisplaywScores(0,10);
    }
    if (goodBadPos[wordPosition] == false)
    {
        cout << "goodWord wordWrong will be called" << endl;
        goodWordWrong(wordPosition,attempt);
        printTop(0,10);
        //goodWords.DisplaywScores(0,10);
        //badWords.DisplaywScores(0,10);
    }
}

void wordCC::goodWordWrong(const int& wordPosition,const std::string& attempt)
{
    //Create new logEvent
    vector<string> eventData;
    word* eventWordp = getWord(wordPosition);
    eventData.push_back(eventWordp->getWord()); //Push back the word in question
    eventData.push_back(to_string(eventWordp->getwScore())); //Add its score
    eventData.push_back(attempt); //And the attempt at the word
    SSG::histLog.addEvent(eventData,time(0),"GoodWordWrong"); //Create the event with the current time stamp

    //Begin main function purpose
    cout << "The word position is " << wordPosition << endl;
    string badWordLine = getWord(wordPosition)->wordWrong(attempt); //Generate the badwordline for the word that was wrong, wordPosition is the wordCC abs index, getword converts to wordContainer abs int then asks for the word behind that abs.
    cout << badWordLine << endl;
    badWord* badWordToAdd = new badWord(getWord(wordPosition),badWordLine); //Create a badword from the word in memory and the generated badwordline
    cout << "bad word Created" << endl;

    int badWordLocation = badWords.addWord(badWordToAdd); //Add the badword to the badwordList

    //cout << "badWordLocation" << badWordLocation << endl;

    int goodWordLocation = wordPos[wordPosition];

    cout << "Begining fixing abstraction" << endl;
    goodWords.deleteWord(goodWords.getABSIndex(wordPos[wordPosition])); //Remove the word from the goodWord list
    goodBadPos[wordPosition] = true; //Set the goodBadPos to say the word is bad
    wordPos[wordPosition] = badWordLocation; //Update the abstraction to point to the location in badWords

    fixwordPos(goodWordLocation);
    //Add overloaded functions that don't include which removing words or deleting words ect. Improve the add words, make indexing turn off able.
}
