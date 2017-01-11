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

	cout << "Tests complete" << endl;
    return 0;
}
