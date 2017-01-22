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
//Early declarations of functinos defined elsewhere
void speak(const string& wordToSay, const bool isCorrect); // src/speech.cpp
void say(const string& sentence); // src/speech.cpp
string seperateWord(const string& wordToSep);
string makeUpperCase(const string& attempt);

void wordCC::userEndSpellingTest()
//Called by GUI when spelling screen return button used
{
    if (currentWord != 0) //If an actual test and not someone exiting after a SpellingOverFlow, or a missclick
    {
        vector<string> dataItems; //Vector to store calculated values
        int timeTaken = time(0) - spellingStart;
        dataItems.push_back(to_string(timeTaken)); //Time taken
        dataItems.push_back(to_string(spellingWrongWordCount)); //Number of incorrect words
        dataItems.push_back(to_string(currentWord)); //Number of words tested
        SSG::histLog.addEvent(dataItems,time(0),"SpellingTestComplete"); //Create the event
    }
}

void wordCC::findSpellingWords()
{
    generatewScore(); //Generate the score of every word in both containers
    findHardest(); //Merge the two containers together
    spellingStart = time(0); //Reset spellingStart
    spellingWrongWordCount = 0; //Reset wrongCount
}

void wordCC::spellingAttempt(const string& attempt)
//Only called when user hits enter and so is sure the answer is correct
{
    bool isCorrect = false; //Used to determine whether 'That is correct, ' should be added to the next thing said
	word* cWord = getCurrentWord(); //Same as getWord(currentWord)
	if (makeUpperCase(attempt) != cWord->getWord()) //If the word spelt incorrectly
	{
		string spelling = seperateWord(cWord->getWord());
		cout << cWord->getWord() << " " << spelling << endl; //Line helps me determine if the program is at fault or me during testing
		string sentenceToSay = "That is incorrect, it is spelt " + spelling; //Create the sentence to be said to the user
		say(sentenceToSay); //Say the sentence above
		wordWrong(currentWord,attempt); //increase MSL values, Add current word to the badWords if a goodWord else increase badword weight
	}
	else
	{
		isCorrect = true;
        wordCorrect(currentWord); //Decrease weighting of word, if a badword with weight of 0 make it good
	}

    nextWord(); //comment to repeatedly ask same word
	speak(getCurrentWord()->getWord(),isCorrect); //Say the next word to the user
}
