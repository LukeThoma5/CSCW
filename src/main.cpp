#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include <gtkmm.h>

#include "../headers/randng.h" //random number generator header
#include "../headers/masterSyllableList.h" //MSL declaration
#include "../headers/masterSyllableListTree.h"
#include "../headers/masterSyllableListMap.h"
#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordContainer.h"
#include "../headers/badwordContainer.h"
#include "../headers/wordCC.h"
#include "../headers/hLog.h"

#include "../objects/windowContainer.cpp"

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
	//wordCC SpellingWords("./Data/finalDictwithDef.txt", "./Data/wrongWords.txt");
	wordCC SpellingWords("./Data/shortDict.txt", "./Data/shortwrongWords.txt");
	hLog histLog("./Data/SSGHistFile.txt");
	time_t sessionStartTime = time(0);
}

masterSyllablesList::~masterSyllablesList() {std::cout<<"ClosingMSL"<<std::endl;}

void say(const string& sentence);
void speak(const string& wordToSay, const bool isCorrect);

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

vector<int> splitVector(const vector<int>& inVector, int mode)
{
	int start;
	int end;
	if (!mode) //if 0
	{
		start = 0;
		end = inVector.size()/2;
	}
	else
	{
		//if not 0
		start = inVector.size()/2;
		end = inVector.size();
	}
	vector<int> returnVector;
	for (int i=start; i<end; i++)
		returnVector.push_back(inVector[i]); //fill the vector with the values from original
	return returnVector;
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

int letterToInt(char c)
{
	return static_cast<int>(c)-65;
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
