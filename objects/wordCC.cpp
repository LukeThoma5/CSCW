#include <ctime>

#include "../headers/wordCC.h"
#include "../headers/hLog.h"

namespace SSG {
	extern hLog histLog;
}

using namespace std;

void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string seperateWord(const string& wordToSep);
string makeUpperCase(const string& attempt);

wordCC::wordCC(std::string goodFilename, std::string badFilename) : goodWords(goodFilename), badWords(goodWords,badFilename)
{
	//wordContainer construction done by Initialisation list above
    cout << "Creating wordContainers from filenames" << endl;
    vector<string> dupWords = badWords.getBadWordList(); //Get all the word strings in the badWord Container
    removeDuplicates(dupWords); //Remove them from the good list
	//They are in the good list to begin with as all the words are loaded from a single file, the badwords are then created from the goodword objects. Once used the goodwords must be removed to prevent a word appearing twice in a single run
}

word* wordCC::getWord(const int& wordToGet)
{
    int wordToGetValue = wordPos[wordToGet];
    //cout << "Trying to getWord " << wordToGetValue << "which has a bool value of " << goodBadPos[wordToGetValue];
    if (goodBadPos[wordToGet] == true) //If badWords
        return badWords.at(wordToGetValue); //Returns an upcasted badword pointer from badWords
    else
        return goodWords.at(wordToGetValue); //Returns a wordpointer from goodwords
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
//Function to get the word position in the alpabetical array, only for goodWords
{
    cout << "Checking goodWords" << endl;
    int location = goodWords.binSearch(comp,0,goodWords.size()); //Do a recursive binary search on goodWords
    cout << location << endl;
    if (location > -1) //if not found print debug info
        cerr << "Word " << comp << " == " << goodWords[location]->getWord() << " at " << location << endl;

    return location; //Return the index
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

word* wordCC::getCurrentWord() {return getWord(currentWord);}

int wordCC::getCurrentPosition() const {return currentWord;}

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
