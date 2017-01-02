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
void GUI_keyboard_Handler(); //Decleration of GUI.cpp function;

int wordCC::notHave30goodWords()
{
    int goodCount=0;
    for (int i=0; i<200; i++)
    {
        if (goodBadPos[i] == false)
            if (++goodCount == 30)
                return 0;
    }
    return 30-goodCount;
}

void wordCC::findKeyboardWords()
{
    cout << "FINDING KEYBOARD WORDS" << endl;
    generatewScore();// Now goodwords and badwords have sorted list
    findHardest();

    //Make the last 150 words random
    for (int i=50; i<200; i++)
    {
        int location = randNG(i,size()-1); //Pick a random location that is further than the current position
        int itemp = wordPos[i]; //Cache the value at the current location
        bool btemp = goodBadPos[i];

        wordPos[i] = wordPos[location]; //Set the current location to the random value
        goodBadPos[i] = goodBadPos[location];

        wordPos[location] = itemp; //Set the random location to the previous value
        goodBadPos[location] = btemp;
    }

    //Check that the 200 words has atleast 300 good words
    int goodShortFall = notHave30goodWords();
    if (goodShortFall) //If not 0 more goodwords to add
    {
        cout << "Adding more goodWords to the test" << endl;
        for (int i=goodWords.size()-1-goodShortFall, j=199-goodShortFall; i<goodWords.size(); i++,j++)
        {
            goodBadPos[j] = false;  //Set the word to be goodBadPos
            wordPos[j] = i;
        }

        //Verify 30 good words
        if (notHave30goodWords())
            cout << "KEYBOARD DOES NOT HAVE 30 GOOD WORDS!" << endl;
    }


    currentWord = 0;
    mistakes = 0;
    keyboardWrongWordCount=0;
    keyboardStart = std::time(0);
    cout << "starting test at " << keyboardStart << endl;
    //printwordCC(20);
}

string wordCC::getKeyboardWords()
{
    cout << "Getting keyboard words" << endl;
    string retString = "";
    for (int i=0; i<200; i++)
    {
        retString += getWord(i)->getWord() + " ";
    }
    return retString;
}

int wordCC::keyboardCharCount()
{
    int charTotal = 0;
    for (int i=0; i<200; i++) //Change to 200 for final release
    {
        charTotal += getCurrentWord()->getWord().size();
    }
    return charTotal;
}

void wordCC::keyboardComplete()
{
    //In future generate dataitems for historyLog
    time_t keyboardEnd = time(0);
    vector<string> eventItems;
    int testLength = keyboardEnd - keyboardStart;
    cout << "#########################################\nKeyboard Test complete!\n";
    cout << "Time taken: " << testLength;

    int charCount = keyboardCharCount();
    cout << "\nTotal characters: " << charCount;

    cout << "\nMistakes: " << mistakes;

    float mistakesPerCharacter = float(mistakes)/float(charCount);
    cout << "\nMistakes per character: " << mistakesPerCharacter;
    cout << "\nMistakes per 100 characters: " << mistakesPerCharacter * 100;

    cout << "\nIncorrect words: " << keyboardWrongWordCount << endl;

    float timeMinutes = testLength/60;
    int wpm = float(200)/timeMinutes;
    cout << "\nWords per minute" << wpm << endl;

    eventItems.push_back(to_string(testLength));
    eventItems.push_back(to_string(charCount));
    eventItems.push_back(to_string(mistakes));
    eventItems.push_back(to_string(mistakesPerCharacter*100));
    eventItems.push_back(to_string(keyboardWrongWordCount));
    eventItems.push_back(to_string(wpm));

    SSG::histLog.addEvent(eventItems,time(0),"keyboardComplete");
    GUI_keyboard_Handler();
}

bool wordCC::keyboardAttempt(const string& attempt)
{
    //cout << "Current word is " << currentWord;
    static string lastString = ""; //initialise value once to empty string
    static bool wordBeenWrong = false;
    const int testEnd = 200;
    if (attempt.back() == ' ')
    {
        cout << "space pressed moving on!" << endl;
        lastString="";
        if (wordBeenWrong)
            keyboardWrongWordCount++;
        if (++currentWord == testEnd) //Increase current word, if at limit end test
            keyboardComplete();
        wordBeenWrong=false;
        return true; //Tell GUI to clear
    }
    cout << "Mistakes: " << mistakes << endl;
    word* currentWordp = getCurrentWord();
    string currentWordString = currentWordp->getWord();
    string attemptUpper = makeUpperCase(attempt);
    if (attemptUpper == currentWordString)
    {
        cout << attemptUpper << " passed!" << endl;
        if (wordBeenWrong)
            keyboardWrongWordCount++;
        if (++currentWord == testEnd) //Change to 200 for final release, 5 is for testing
            keyboardComplete();
        lastString="";
        wordBeenWrong=false;
        return true;
    }

    if (lastString.size()>attemptUpper.size())
        return false; //If trying to remove mistake, don't penalise

    if (attemptUpper.back() != currentWordString[attemptUpper.size()-1])
    {
        cout << "last character incorrect, adding a mistake" << endl;
        mistakes++;
        wordBeenWrong=true;
    }

    if (attemptUpper.size() == currentWordString.size())
    {
        if (wordBeenWrong)
            keyboardWrongWordCount++;
        if (++currentWord == testEnd) //Change to 200 for final release, 5 is for testing
            keyboardComplete();
        lastString="";
        wordBeenWrong=0;
        return true;
    }

    lastString=attemptUpper;
    return false;

}
