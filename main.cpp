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
	wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt");
	hLog histLog("SSGHistFile.txt");
	time_t sessionStartTime = time(0);
	//wordCC SpellingWords("shortDict.txt", "wrongWords.txt");
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

void connectSignals();

int main (int argc, char **argv)
{

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    //Load the GtkBuilder file and instantiate its widgets:
    SSG::refBuilder = Gtk::Builder::create();
    try
    {
      SSG::refBuilder->add_from_file("SSG_Gui_rev2.glade");
    }
    catch(const Glib::FileError& ex)
    {
      std::cerr << "FileError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
      std::cerr << "MarkupError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
      std::cerr << "BuilderError: " << ex.what() << std::endl;
      return 1;
    }

	SSG::winContainer.addWindows(SSG::refBuilder);
	connectSignals();

    //Get the GtkBuilder-instantiated Dialog:
	//Gtk::Window* MainScreen;

	//refBuilder->get_widget("SSG_Spelling_Screen", pDialog);
	//refBuilder->get_widget("SSG_Main_Screen", MainScreen);


	/*
	SSG::MSL = masterSyllablesList();
	SSG::goodWordList = NULL;
	SSG::badWordList = NULL;
	*/

	/*
	vector<string> dictVec = loadDictFile();

	//cout << dictVec[0] << endl;
	//cout << dictVec[dictVec.size()-1] << endl;

	vector<word> wordList;

	//Load all the words from the word list
	for (unsigned int i=0; i<dictVec.size(); i++)
	{
		//word* newWord = new word(dictVec[i]);
		//wordList.push_back(*newWord);
		//delete newWord;
		wordList.push_back(word(dictVec[i]));
	}

	/*
	for (unsigned int i=0; i<3; i++)
	{
		cout << wordList[i].wordC  << ": "<< wordList[i].definition << endl;
	}
	*/

	wordContainer allWords("finalDictwithDef.txt");


	SSG::goodWordList = &allWords;
	//SSG::badWordList = &allBadWords;

	cout << SSG::goodWordList->wordList[1]->wordC << endl;

	printVector(SSG::MSL.syllables);

	// printVector(SSG::MSL.wrongCount);

	createRandomWordWrongCounts();

	printVector(SSG::MSL.wrongCount);

	SSG::MSL.sortList();

	cout << "List sorted ---------------------------------------------" << endl;

	//printVector(SSG::MSL.syllables);
	//printVector(SSG::MSL.wrongCount);
	//cout << findWordLocation("ALLURING",wordList) << endl;
	//cout << allWords.findWordLocation("BOIL") << endl;
	//cout << SSG::goodWordList->findWordLocation("BOIL") << endl;
	//int boilLocation = allWords.findWordLocation("ADEQUATELY");
	//cout << boilLocation << endl;
	//cout << allWords.wordList[boilLocation]->wordC << endl;
	//allWords.wordList[boilLocation]->wordWrong("ADEKUAT3IYY",SSG::badWordList);
	//allWords.wordWrong(boilLocation,"ADEKUAT3IYY",&allWords);
	//badWord* test2 = SSG::badWordList->getBadWord(0);
	//printVector(SSG::badWordList->getBadWord(0)->syllableWrongCount);
	//printVector(static_cast<badWord*>(SSG::badWordList->wordList[0])->syllableWrongCount);
	//wordCC SpellingWords("shortDict.txt", "wrongWords.txt");

	//SSG::SpellingWords.generatewScore();

	//SSG::SpellingWords.findHardest();

	//SSG::SpellingWords.printTop();


	//callTest(allWords.wordList[0]);
	//speak(SpellingWords[0]->getWord());
	//speak(SpellingWords[1]->getWord());
	//string toSearch = "ADEQUATELY";
	//SpellingWords.findRealWordLocation(toSearch);
	//string testString = "ATTEMPTO+AH0+T+EH1+M+P+T+OW1+#DEF+The latin for test";
	//SpellingWords.addWord(new word(testString));
	//SpellingWords.wordWrong(0,"SNAPY");
	//word* wordToGetWrong = SpellingWords[4];
	//SpellingWords.wordWrong(4,wordToGetWrong->getWord().substr(1,wordToGetWrong->getWord().size()));
	//SpellingTest(SpellingWords);
	//SpellingTest(SpellingWords);

	//app->run(*pDialog);
	app->run(*(SSG::winContainer.MainScreen));


	//delete pDialog;

	cout << "Returning to OS" << endl;

	//Add to badWord construction that it should delete the word from the good list wordCC construction

	return 0;
}
