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
//#include "../headers/SSG.h"
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

void wordCC::userEndSpellingTest()
{
    if (currentWord != 0) //If an actual test and not someone exiting after a SpellingOverFlow
    {
        vector<string> dataItems;
        int timeTaken = time(0) - spellingStart;
        dataItems.push_back(to_string(timeTaken)); //Time taken
        dataItems.push_back(to_string(spellingWrongWordCount)); //Number of incorrect words
        dataItems.push_back(to_string(currentWord)); //Number of words tested
        SSG::histLog.addEvent(dataItems,time(0),"SpellingTestComplete");
    }
}

void wordCC::findSpellingWords()
{
    generatewScore();
    findHardest();
    spellingStart = time(0); //Reset spellingStart
    spellingWrongWordCount = 0; //Reset wrongCount
}

void wordCC::spellingAttempt(const string& attempt)
{
    bool isCorrect = false;
	word* cWord = getCurrentWord();
	if (makeUpperCase(attempt) != cWord->getWord())
	{
		cout << "The correct spelling is " << cWord->getWord() << endl;
		string spelling = seperateWord(cWord->getWord());
		cout << cWord->getWord() << " " << spelling << endl;
		string sentenceToSay = "That is incorrect, it is spelt " + spelling;
		//EntryBuffer->set_text("The correct spelling is: " + cWord->getWord() + " not " + attempt);
		say(sentenceToSay);
		wordWrong(currentWord,attempt);
	}
	else
	{
		isCorrect = true;
        wordCorrect(currentWord);
	}
	//nextWord();
    nextWord(); //comment to repeatedly ask same word
	speak(getCurrentWord()->getWord(),isCorrect);
}
