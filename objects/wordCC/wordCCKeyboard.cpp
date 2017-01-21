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
	extern hLog histLog; //Needed to be able to create keyboard end event
}

using namespace std;

string makeUpperCase(const string& attempt); //Returns string in uppercase format
void GUI_keyboard_Handler(); //Decleration of GUI.cpp function;

int wordCC::notHave30goodWords()
{
	//Only called if wordCC.size() > 200 by GUI
    int goodCount=0; //Number of words not gotten wrong
    for (int i=0; i<200; ++i)
	//For the first 200 words of the wordList
    {
        if (goodBadPos[i] == false) //If a good word
            if (++goodCount == 30) //If the incremented value of goodCount is 30
                return 0; //Return 0 need to be added
    }
    return 30-goodCount; //Return the amount found of goodwords that need adding
}

void wordCC::findKeyboardWords()
{
    cout << "FINDING KEYBOARD WORDS" << endl;
    generatewScore();// Now goodwords and badwords have sorted lists
    findHardest(); //Generate the wordCC abstraction

    //Make the last 150 words random
    for (int i=50; i<200; ++i)
    {
        int location = randNG(i,size()-1); //Pick a random location that is further than the current position (so it can't override a value that has already been randomised)
        int itemp = wordPos[i]; //Cache the value at the current location
        bool btemp = goodBadPos[i]; //Cache whether it is good or bad

        wordPos[i] = wordPos[location]; //Set the current location to the random value
        goodBadPos[i] = goodBadPos[location];

        wordPos[location] = itemp; //Set the random location to the previous value
        goodBadPos[location] = btemp;
    }

    //Check that the 200 words has atleast 30 good words
	//notHave30goodWords was going to be a bool return but int allows for more useful information
	//Which is why the name is a bit odd.
    int goodShortFall = notHave30goodWords();
    if (goodShortFall) //If not 0 more goodwords to add
    {
        cout << "Adding more goodWords to the test" << endl;
        for (int i=goodWords.size()-1-goodShortFall, j=199-goodShortFall; i<goodWords.size(); i++,j++)
		//From the back of goodWords abstraction add the back of the 200 words
        {
            goodBadPos[j] = false;  //Set the word to be goodBadPos
            wordPos[j] = i; //Set the location to point to a word in goodWords
        }

        //Verify 30 good words
        if (notHave30goodWords())
			//Should never execute
            cerr << "KEYBOARD DOES NOT HAVE 30 GOOD WORDS!" << endl;
    }


    currentWord = 0; //Reset current word
    mistakes = 0; //If second keyboard test, remove previous mistakes
    keyboardWrongWordCount=0;
    keyboardStart = std::time(0);
    cout << "starting test at " << keyboardStart << endl;
}

string wordCC::getKeyboardWords()
{
	//Only run if more that 200 words in wordCC
    cout << "Getting keyboard words" << endl;
    string retString = "";
    for (int i=0; i<200; ++i) //For 200 words in test
        retString += getWord(i)->getWord() + " "; //Add the word and a seperator
    return retString;
}

int wordCC::keyboardCharCount()
{
    int charTotal = 0;
    for (int i=0; i<200; ++i)
        charTotal += getWord(i)->getWord().size();
		//Get the word* behind the index, get its wordC and add its size to the charTotal
    return charTotal;
}

void wordCC::keyboardComplete()
{
    //Cache the end time
    time_t keyboardEnd = time(0);
    vector<string> eventItems; //Vector to store the event item strings in
	//Calculate the event data items
    int testLength = keyboardEnd - keyboardStart; //KeyboardEnd is the bigger number (later in time)
    int charCount = keyboardCharCount();
    float mistakesPerCharacter = float(mistakes)/float(charCount);
    float timeMinutes = testLength/60;
    int wpm = float(200)/timeMinutes;
    //Add the event items to the vector
    eventItems.push_back(to_string(testLength));
    eventItems.push_back(to_string(charCount));
    eventItems.push_back(to_string(mistakes));
    eventItems.push_back(to_string(mistakesPerCharacter*100));
    eventItems.push_back(to_string(keyboardWrongWordCount));
    eventItems.push_back(to_string(wpm));

	#ifdef WORDCCDEBUG
	//Only compile debugging info if WORDCCDEBUG active
	cout << "\nTotal characters: " << charCount;
    cout << "\nMistakes: " << mistakes;
	cout << "\nMistakes per character: " << mistakesPerCharacter;
	cout << "\nMistakes per 100 characters: " << mistakesPerCharacter * 100;
	cout << "\nIncorrect words: " << keyboardWrongWordCount << endl;
	cout << "\nWords per minute" << wpm << endl;
	cout << "#########################################\nKeyboard Test complete!\n";
    cout << "Time taken: " << testLength;
	#endif
	//Generate the event based of calculated data
    SSG::histLog.addEvent(eventItems,time(0),"keyboardComplete");
    GUI_keyboard_Handler(); //Update the GUI for a new keyboard session (contains a call to findKeyboardWords)
}

bool wordCC::keyboardAttempt(const string& attempt)
{
    //This function is called every time the text changes be an insertion or removal
	//Persistant variables between function calls, reset when moving on to next word
    static string lastString = ""; //initialise value once to empty string
    static bool wordBeenWrong = false;
    const int testEnd = 200; //Evaluated at compile time so no runtime overhead. Used to shorted tests to make it easier to test correct completion
    if (attempt.back() == ' ') //If they inserted a space, skip to next word
    {
        cout << "space pressed moving on!" << endl;
        lastString=""; //Reset the string
        if (wordBeenWrong) //If they made a mistake
            keyboardWrongWordCount++; //Add to number of wrong words
        if (++currentWord == testEnd) //Increase current word, if at limit end test
            keyboardComplete();
        wordBeenWrong=false; //Reset static variable
        return true; //Tell GUI to clear
    }
    cout << "Mistakes: " << mistakes << endl;
    word* currentWordp = getCurrentWord(); //Cache current word to reduce abstraction calls
    string currentWordString = currentWordp->getWord();
    string attemptUpper = makeUpperCase(attempt);
    if (attemptUpper == currentWordString) //If word completely correct
    {
        cout << attemptUpper << " passed!" << endl;
        if (wordBeenWrong) //If they made a mistake at any point
            keyboardWrongWordCount++; //Increase the number of words which had a mistake
        if (++currentWord == testEnd) //Increase current word, if at limit end test
            keyboardComplete();
        lastString=""; //Reset
        wordBeenWrong=false; //Reset
        return true; //Tell GUI to clear
    }

    if (lastString.size()>attemptUpper.size())
        return false; //If trying to remove mistake, don't penalise

	//If the last inserted letter is not the same as the letter in the word
    if (attemptUpper.back() != currentWordString[attemptUpper.size()-1])
    {
        cout << "last character incorrect, adding a mistake" << endl;
        mistakes++; //Add a mistake
        wordBeenWrong=true; //Mark the word as one that has been wrong at least once
    }
	//If the attempt is the same size move on
    if (attemptUpper.size() == currentWordString.size())
    {
        if (wordBeenWrong)
            keyboardWrongWordCount++;
        if (++currentWord == testEnd)
            keyboardComplete();
        lastString="";
        wordBeenWrong=0;
        return true;
    }

    lastString=attemptUpper;
    return false; //Tell GUI don't clear, continue going
}
