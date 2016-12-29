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
#include "masterSyllableListTree.h"
#include "masterSyllableListMap.h"
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
	masterSyllablesListTree physicalMSL;
	//masterSyllablesListMap physicalMSL;
	masterSyllablesList* MSL = &physicalMSL; //MasterSyllablesList
	wordContainer* goodWordList = NULL; //Allows the currently worked on wordList to be updated and interchanged
	wordContainer* badWordList = NULL; //Defaulting to null prevents manipulating objects that don't exist.
	Glib::RefPtr<Gtk::Builder> refBuilder;
	windowContainer winContainer;
	// wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt");
	wordCC SpellingWords("shortDict.txt", "shortwrongWords.txt");
	hLog histLog("SSGHistFile.txt");
	time_t sessionStartTime = time(0);
}

masterSyllablesList::~masterSyllablesList() {std::cout<<"ClosingMSL"<<std::endl;}


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
	for (unsigned int i=start; i < sV.size(); i++)
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

	for (unsigned int i=0; i<SSG::MSL->size(); i++)
	{
		//SSG::MSL.wrongCount[i] = randNG(LOW,HIGH); //Valgrind does not like this call
		SSG::MSL->addToTotal((*SSG::MSL)[i],randNG(LOW,HIGH)); //Use just the interface
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

string makeSalt()
{
	string salt;
	for (int i=0; i<256; i++)
	{
		salt += static_cast<char>(randNG(65,128));
	}
	cout << salt << endl;
	return salt;
}

string saltPassword(const string& password, const string& salt)
{
	string saltedPass = "";
	int passSize = password.size();
	int saltSize = salt.size();
	bool addMoreSalt = true;
	int end = passSize;
	if (passSize != saltSize)
	{
		if (passSize > saltSize)
		{
			end = saltSize;
			addMoreSalt = false;
		}
	}

	for (int i=0; i<end; i++)
	{
		saltedPass += password[i] + salt[i];
	}

	if (passSize != saltSize)
		if (addMoreSalt)
			saltedPass += salt.substr(end,saltSize);
		else
			saltedPass += password.substr(end,passSize);

	return saltedPass;
}

vector<string> readPasswordFile()
{
	ifstream passFile("userPassword.hash");
	vector<string> retVec;
	string line;
	while (getline(passFile, line))
	{
		retVec.push_back(line);
	}
	return retVec;
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


	// printVector(SSG::MSL.wrongCount);

	if (SSG::MSL->hasNoValues())
		createRandomWordWrongCounts();

	//printVector(SSG::MSL.syllables);
	//printVector(SSG::MSL.wrongCount);
	SSG::MSL->print();

	SSG::MSL->sortList();

	cout << "List sorted ---------------------------------------------" << endl;

	app->run(*(SSG::winContainer.MainScreen));


	cout << "Returning to OS" << endl;

	//Add to badWord construction that it should delete the word from the good list wordCC construction

	return 0;
}
