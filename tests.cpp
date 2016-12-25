#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

//Needed for say functin and pipe functionality
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <gtkmm.h>

#include "randng.h" //My random number generator header
#include "masterSyllableList.h" //MSL declaration
#include "word.h"
#include "badWord.h"
#include "SSG.h"
#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"
#include "hLog.h"

#include "windowContainer.cpp"

using namespace std;

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	masterSyllablesList MSL; //MasterSyllablesList
	wordContainer* goodWordList = NULL; //Allows the currently worked on wordList to be updated and interchanged
	wordContainer* badWordList = NULL; //Defaulting to null prevents manipulating objects that don't exist.
	Glib::RefPtr<Gtk::Builder> refBuilder;
	windowContainer winContainer;
	// wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt");
	wordCC SpellingWords("shortDict.txt", "shortwrongWords.txt");
	hLog histLog("SSGHistFile.txt");
	time_t sessionStartTime = time(0);
}



void say(const string& sentence)
{
	/* //Old say function
	//string Command = "flite -voice slt -t \"" + sentence + "\"";
	string Command = "flite -t \"" + sentence + "\"";

    system(Command.c_str());
	*/

	int fd;
    std::string myfifo = "/tmp/myfifo";
    //char * myfifo = "/tmp/myfifo";

    /* create the FIFO (named pipe) */
    mkfifo(myfifo.c_str(), 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo.c_str(), O_WRONLY);
    //write(fd, argv[1], sizeof(argv[1]));
    write(fd,sentence.c_str(),100);
    close(fd);

    /* remove the FIFO */
    unlink(myfifo.c_str());
}

string makeUpperCase(const string& attempt)
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

void speak(const string& wordToSay, const bool isCorrect)
{
	/*
	//string Command = "flite -voice slt -t \"Please spell the word " + wordToSay + "\"";
	string Command = "flite -t \"Please spell the word " + wordToSay + "\"";
    system(Command.c_str());
	*/
	if (isCorrect)
		say("That is correct, please spell the word " + wordToSay);
	else
		say("Please spell the word " + wordToSay);
}

void printVector(const vector<string>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=0; i < sV.size(); i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void printVector(const vector<int>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=start; i < stop; i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void printVector(const vector<bool>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=start; i < stop; i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void createRandomWordWrongCounts() //Fill the MSL with random wrongCounts to test sorting
{
	const int HIGH = 12; //Was 450
	const int LOW = 0;

	for (unsigned int i=0; i<SSG::MSL.wrongCount.size(); i++)
	{
		SSG::MSL.wrongCount[i] = randNG(LOW,HIGH); //Valgrind does not like this call
		//SSG::MSL.wrongCount[i] = 0; //This can be used to memory leak testing
	}
}

int randNG(int low, int high)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(low,high);
	return dist6(rng);
}

int letterToInt(char c)
{
	return static_cast<int>(c)-65;
}

word& returnRef(word* myWord)
{
	return *myWord;
}

void SpellingTest(wordCC& SpellingWords)
{
	SpellingWords.generatewScore();

	SpellingWords.findHardest();

	for (int i=0; i<10; i++)
	{
		//SpellingWords.printwordCC();
		string wordToSpell = SpellingWords[i]->getWord();
		speak(wordToSpell,false);
		string userInput;
		cin >> userInput;
		if (userInput == "exit")
			exit(0);
		if (userInput != wordToSpell)
		{
			cout << "You spelt it: " << userInput << " correct spelling is " << wordToSpell << endl;
			SpellingWords.wordWrong(i,userInput);
		}
		else
		{
			cout << "Correct!" << endl;
		}

	}
}

bool testWordContainerSearch(wordContainer& goodWords)
{
    //string.compare has issues when passed antic and anti-Federalist
    //bool success = true;
    int j = 0;
    for (int i=0; i<goodWords.size(); i++)
    {
        //cout << "### Now searching for " << goodWords[i]->getWord() << endl;
        int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
        if (location == -1)
        {
            cout << "Word not found" << endl;
            return false;
        }
        if (i != location)
        {
            cout << "Binary Search Test [Failed]" << endl;
            cout << "An error has occured " << i << " != " << location << endl;
            return false;
        }
        j++;
        if (j > 1000)
        {
            int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
            //cout << "Looking for word # " << i << " with value " << goodWords[i]->getWord() << " which was found at location " << location << " with a value of " << goodWords[location]->getWord() << endl;
            j=0;
        }
    }
    //cout << "Binary Search Test [Passed]" << endl;
    return true;
}

bool testMSLMap(const string& testType)
{
	for (int i=0; i<SSG::MSL.size(); i++) //For every syllable in the masterSyllableList
	{
		string testSyllable = SSG::MSL[i];
		//cout << SSG::MSL.getSyllableWCount(i) << "  " << SSG::MSL.wrongCount[i] << '\n';
		//cout << "Testing syllable " << testSyllable << " at position " << i << " does " << SSG::MSL.getSyllableWCount(i) << "==" << SSG::MSL.getSyllableWCount(testSyllable) << "?\n";
		if (SSG::MSL.getSyllableWCount(i) != SSG::MSL.getSyllableWCount(testSyllable)) //If there is a mismatch in the returned wrongCount
		{
			cout << "Error MSL position and value do mismatch!\nTest syllable " << testSyllable << " at position " << i << endl;
			cout << testType << "[Failed]" << endl;
			return false;
		}
	}
	cout << testType << "[Passed]" << endl;
}

void printMSL()
{
	for (int i=0; i<SSG::MSL.size(); i++)
	{
		cout << SSG::MSL[i] << '[' << SSG::MSL.getSyllableWCount(i) << " :: " << SSG::MSL.getSyllableWCount(SSG::MSL[i]) << "]\n";
	}
}

int main(int argc, char const *argv[]) {
    wordContainer goodWords("finalDictwithDef.txt");
    //wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt
	testWordContainerSearch(goodWords);
	createRandomWordWrongCounts();
	printMSL();
	testMSLMap("MSL Map Creation Test ");
	SSG::MSL.sortList();
	testMSLMap("Merge sorted Map Test ");
	printMSL();
	cout << "Tests complete" << endl;
    return 0;
}
