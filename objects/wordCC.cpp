#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <ctime>

#include "../headers/wordContainer.h"
#include "../headers/badwordContainer.h"
#include "../headers/wordCC.h"
#include "../headers/hLog.h"
#include "../headers/randng.h"

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
                if (goodWords.at(leftPos)->getwScore() >= badWords.at(rightPos)->getwScore()) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
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
		int dupLocation;
		try {dupLocation = goodWords.binSearch(dupWords[i],0,goodWords.size());}
		catch (int errorNum)
		{
			dupLocation = -1;
			cout << "Failed to find word to delete, binSearch returned with code " << errorNum << endl;
		}
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

int wordCC::getCurrentPosition() const
{
    return currentWord;
}

int wordCC::size() const {return goodWords.size()+badWords.size();}

void wordCC::nextWord()
{
    if (++currentWord == (goodWords.size()+badWords.size())) //Increase the currentword THEN compare it with the total amount of words
    {
        //If out of words, reset currentWord and regenerateSpellingWords
        currentWord = 0; //Reset word to the start

        //Create the overflow event
        time_t currentTime = time(0);
        vector<string> dataItems;
        int timeTaken = currentTime - spellingStart;
        dataItems.push_back(to_string(timeTaken)); //Time taken
        dataItems.push_back(to_string(goodWords.size())); //Amount of goodwords
        dataItems.push_back(to_string(badWords.size())); //Amount of badWords
        SSG::histLog.addEvent(dataItems,currentTime,"SpellingOverFlow"); //Add the event to the log

        //Regenerate scores
        findSpellingWords();
    }
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
