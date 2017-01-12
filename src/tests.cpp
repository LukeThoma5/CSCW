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
	//masterSyllablesListTree physicalMSL;
	masterSyllablesListMap physicalMSL;
	masterSyllablesList* MSL = &physicalMSL; //MasterSyllablesList
	Glib::RefPtr<Gtk::Builder> refBuilder;
	windowContainer winContainer;
	//wordCC SpellingWords("./Data/finalDictwithDef.txt", "./Data/wrongWords.txt");
	wordCC SpellingWords("./Data/shortDict.txt", "./Data/shortwrongWords.txt");
	hLog histLog("./Data/SSGHistFile.txt");
	time_t sessionStartTime = time(0);
}

void createRandomWordWrongCounts();
void printVector(const vector<string>& sV, int start=0, int stop=-1);
void printVector(const vector<int>& sV, int start=0, int stop=-1);

void runMSLSortingTests();
void runMSLaddToTotalTest();
void runtestMSLoverridedSWC();
void runCompleteMSLADDSyllables();
bool testWordContainerSearch(wordContainer& goodWords);
void runTestWordContainerStringCompare(wordContainer& goodWords);
void runAllhLogTests();

void logEventGenEventLineTest()
{
	string EventLine = "585a9fe5+GoodWordWrong+ABACK+3.863636+abba";
	logEvent lE(EventLine);
	if (lE.generateEventLine() == EventLine)
		cout << "logEvent eventLine from eventLine test [Passed]" << endl;
	else
		cout << "logEvent eventLine from eventLine test [Failed]" << endl;

	logEvent lETwo({"ABACK","3.863636","abba"},0x585a9fe5,"GoodWordWrong");
	if (lETwo.generateEventLine() == EventLine)
		cout << "logEvent eventLine from dataItems test [Passed]" << endl;
	else
		cout << "logEvent eventLine from dataItems test [Failed]" << endl;
}

int main(int argc, char const *argv[]) {
    wordContainer goodWords("./Data/finalDictwithDef.txt");
    //wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt
	testWordContainerSearch(goodWords);
	runTestWordContainerStringCompare(goodWords);
	createRandomWordWrongCounts();
	runMSLSortingTests();
	runMSLaddToTotalTest();
	runtestMSLoverridedSWC();
	runCompleteMSLADDSyllables();
	logEventGenEventLineTest();
	runAllhLogTests();

	cout << "Tests complete" << endl;
    return 0;
}
