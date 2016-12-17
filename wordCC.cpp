#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <ctime>

#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"
#include "hLog.h"
#include "SSG.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
void printVector(const std::vector<bool>& sV, int start=0, int stop=-1);
void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string seperateWord(const string& wordToSep);

wordCC::wordCC()
{
    cout << "Default wordCC constructor called!" << endl;
}

void wordCC::fixwordPos(const int& goodWordLocation)
{
    for (int i=0; i<wordPos.size(); i++)
    {
        if (goodBadPos[i] == false)
        {
            if (wordPos[i] > goodWordLocation)
                wordPos[i]--;
        }
    }
}

void wordCC::goodWordWrong(const int& wordPosition,const std::string& attempt)
{
    //cout << wordList[wordPosition]->wordC << endl;
    cout << "The word position is " << wordPosition << endl;
    string badWordLine = getWord(wordPosition)->wordWrong(attempt); //Generate the badwordline for the word that was wrong, wordPosition is the wordCC abs index, getword converts to wordContainer abs int then asks for the word behind that abs.
    cout << badWordLine << endl;
    badWord* badWordToAdd = new badWord(getWord(wordPosition),badWordLine); //Create a badword from the word in memory and the generated badwordline
    cout << "bad word Created" << endl;
    //cout << badWordToAdd->wordFlags[0] << endl;
    int badWordLocation = badWords.addWord(badWordToAdd); //Add the badword to the badwordList

    //cout << "badWordLocation" << badWordLocation << endl;

    int goodWordLocation = wordPos[wordPosition];

    cout << "Begining fixing abstraction" << endl;
    goodWords.deleteWord(goodWords.getABSIndex(wordPos[wordPosition]));
    goodBadPos[wordPosition] = true;
    wordPos[wordPosition] = badWordLocation;

    fixwordPos(goodWordLocation);


    //Add overloaded functions that don't include which removing words or deleting words ect. Improve the add words, make indexing turn off able.
}

void wordCC::combineWordLists()
{
    //This assumes that generatewScore has already been called so the two lists are already sorted themselves.
    int leftPos = 0; //Simple vector position pointers
    int rightPos = 0;

    int leftVectorSize = goodWords.size();
    int rightVectorSize = badWords.size();

    while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
    {
        if (leftPos == leftVectorSize) //If out of left hand values add a right
        {
            wordPos.push_back(rightPos++); //add the next right value to the return list and increment rightPos counter
            goodBadPos.push_back(true);
        }
        else
        {
            if (rightPos == rightVectorSize) //If out of right hand values add a left
            {
                //cout << "Adding word " << leftPos << true;
                wordPos.push_back(leftPos++);
                goodBadPos.push_back(false);
            }

            else
            {
                if (goodWords.at(leftPos)->getwScore() >= badWords.at(leftPos)->getwScore()) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
                {
                    //cout << "Adding word " << leftPos << true;
                    wordPos.push_back(leftPos++); //Add the leftvector position
                    goodBadPos.push_back(false);
                }
                else
                {
                    wordPos.push_back(rightPos++);
                    goodBadPos.push_back(true);
                }
            }
        }
    }

    printVector(wordPos,0,10);
    printVector(goodBadPos,0,10);

    //cout << "The difference in locations is " << wordPos.size()-goodBadPos.size() << endl;
}

void wordCC::removeDuplicates(const vector<string>& dupWords)
{
    for (unsigned int i=0; i<dupWords.size(); i++)
    {
        //Search goodWords for this iterations badWord, this is important as the badWord is constructed from the goodword which must then be removed.
        int dupLocation = goodWords.binSearch(dupWords[i],0,goodWords.size());
        cout << "Duplicate word " << dupWords[i] << " found in word container at location " << dupLocation << endl;
        //Delete the located word
        if (dupLocation != -1)
            goodWords.deleteWord(dupLocation);
    }
}

wordCC::wordCC(std::string goodFilename, std::string badFilename) : goodWords(goodFilename), badWords(goodWords,badFilename)
{
        cout << "Creating wordContainers from filenames" << endl;

        //Improve addWord, make it virtual and add a version for badwordContainer.
        vector<string> dupWords = badWords.getBadWordList();
        removeDuplicates(dupWords);
}

void wordCC::generatewScore()
{
    cout << "Determining Scores" << endl;

    //goodWords[8122]->determineScore();
    goodWords.sortWordContainer();
    badWords.sortWordContainer();

    //goodWords.DisplaywScores(100,120);

    badWords.DisplaywScores();

}

void wordCC::clearWordPos()
{
    wordPos.clear(); //Removes previous positions
    int totalSize = goodWords.size() + badWords.size();
    wordPos.reserve(totalSize); //Reduces the need to move the vector in memory by saying out the gate the miniumum space needed
    goodBadPos.clear();
    goodBadPos.reserve(totalSize);
    //for (unsigned int i=0; i<totalSize; i++)
    //{
    //    wordPos.push_back(0);
    //}
}

void wordCC::findHardest()
{
    clearWordPos();
    //totalSize = goodWords.size() + badWords.size();
    combineWordLists();
    currentWord=0;
}

word* wordCC::getWord(const int& wordToGet)
{
    int wordToGetValue = wordPos[wordToGet];
    //cout << "Trying to getWord " << wordToGetValue << "which has a bool value of " << goodBadPos[wordToGetValue];
    if (goodBadPos[wordToGet] == true) //If badWords
        return badWords.at(wordToGetValue);
    else
        return goodWords.at(wordToGetValue);
}

badWord* wordCC::getBWord(const int& wordToGet)
{
    if (goodBadPos[wordToGet] == true)
        return badWords.at(wordPos[wordToGet]);
    else
        throw "Not a bad word!";
}

void wordCC::printTop(int start, int end)
{
    if (end > (goodWords.size() + badWords.size()))
        end = (goodWords.size() + badWords.size());
    for (unsigned int i=start; i<end; i++)
    {
        word* wordToPrint = getWord(i);
        cout << i << ": " << wordToPrint->getWord() << " with a score of " << wordToPrint->getwScore() << '\n';
    }
    cout << endl;
}

int wordCC::findRealWordLocation(const string& comp)
{
    cout << "Checking goodWords" << endl;
    int location = goodWords.binSearch(comp,0,goodWords.size());
    cout << location << endl;
    if (location > -1)
        cout << "Word " << comp << " == " << goodWords[location]->getWord() << " at " << location << endl;

    return location;
}

void wordCC::wordWrong(const int& wordPosition,const string& attempt)
{
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

void wordCC::addWord(word* wordToAdd)
{
    //Add code to fix wordCC not wordContainer abstraction?
    goodWords.addWord(wordToAdd);
}

void wordCC::printwordCC(int stop)
{
    if (stop == 0)
        stop = goodWords.size()+badWords.size();
    else
    {
        if (stop > (goodWords.size()+badWords.size()))
            stop = goodWords.size()+badWords.size();
    }
    cout << "PRINTING WORDCC\n";
    for (int i=0; i<stop; i++)
    {
        cout << "Word " << i;
        if (goodBadPos[i] == true)
            cout << " is a bad word ";
        else
            cout << " is a good word ";
        cout << getWord(i)->getWord() << " with a value " << getWord(i)->getwScore() << endl;
    }
    cout << "\n\nPRINTING GOODWORDS\n";
    goodWords.printWordContainer();
    cout << "\n\nPRINTING BADWORDS\n";
    badWords.printWordContainer();
}

word* wordCC::getCurrentWord()
{
    return getWord(currentWord);
}

int wordCC::getCurrentPosition()
{
    return currentWord;
}

void wordCC::nextWord()
{
    if (++currentWord == (goodWords.size()+badWords.size())) //Increase the currentword THEN compare it with the total amount of words
    {
        //If out of words, reset currentWord and regenerateSpellingWords
        currentWord = 0;
        findSpellingWords();
    }
}

void wordCC::findSpellingWords()
{
    generatewScore();
    findHardest();
}

bool wordCC::notHave30goodWords()
{
    int goodCount=0;
    for (int i=0; i<200; i++)
    {
        if (goodBadPos[i] == false)
            if (++goodCount == 30)
                return false;
    }
    return true;
}

void wordCC::findKeyboardWords()
{
    cout << "FINDING KEYBOARD WORDS" << endl;
    generatewScore();// Now goodwords and badwords have sorted list
    findHardest();
    if (notHave30goodWords())
        cout << "SOMETHINE HAS GONE HORRIBLY WRONG IN findKeyboardWords!" << endl;

    currentWord = 0;
    mistakes = 0;
    keyboardWrongWordCount=0;
    keyboardStart = std::time(0);
    cout << "starting test at " << keyboardStart << endl;
    //printwordCC(20);
}

string wordCC::getKeyboardWords()
{
    string retString = "";
    for (int i=0; i<200; i++)
    {
        retString += getWord(i)->getWord() + " ";
    }
    return retString;
}

string wordCC::makeUpperCase(const string& attempt)
{
    //Should be ok but some shortcuts were made for efficency. May cause errors
    string upperString;
    for (unsigned int i=0; i<attempt.size(); i++)
    {
        char currentChar = attempt[i];
        int charInt = static_cast<int>(currentChar);
        if (charInt>96)
        {
            charInt -= 32;
        }
        upperString += static_cast<char>(charInt);
    }
    return upperString;
}

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
                if (i>wordPosition)
                    wordPos[i]--;
        }

        printwordCC();
    }
    else
    {
        cout << "word still got score > 0.0" << endl;
    }

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

void GUI_keyboard_Handler(); //Decleration of GUI.cpp function;

int wordCC::keyboardCharCount()
{
    int charTotal = 0;
    for (int i=0; i<5; i++) //Change to 200 for final release
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
    eventItems.push_back(to_string(testLength));
    cout << "\nMistakes: " << mistakes;
    eventItems.push_back(to_string(mistakes));
    int charCount = keyboardCharCount();
    cout << "\nTotal characters: " << charCount;
    eventItems.push_back(to_string(charCount));
    float mistakesPerCharacter = float(mistakes)/float(charCount);
    eventItems.push_back(to_string(mistakesPerCharacter));
    cout << "\nMistakes per character: " << mistakesPerCharacter;
    cout << "\nIncorrect words: " << keyboardWrongWordCount << endl;
    SSG::histLog.addEvent(eventItems,time(0),"keyboardComplete");
    GUI_keyboard_Handler();
}

bool wordCC::keyboardAttempt(const string& attempt)
{
    //cout << "Current word is " << currentWord;
    static string lastString = ""; //initialise value once to empty string
    static bool wordBeenWrong = false;
    if (attempt.back() == ' ')
    {
        cout << "space pressed moving on!" << endl;
        lastString="";
        if (wordBeenWrong)
            keyboardWrongWordCount++;
        if (++currentWord == 5) //Increase current word, if at limit end test
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
        if (++currentWord == 5) //Change to 200 for final release, 5 is for testing
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
        if (++currentWord == 5) //Change to 200 for final release, 5 is for testing
            keyboardComplete();
        lastString="";
        wordBeenWrong=0;
        return true;
    }

    lastString=attemptUpper;
    return false;

}
