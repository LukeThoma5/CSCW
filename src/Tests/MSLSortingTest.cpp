#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include <gtkmm.h>

#include "../../headers/randng.h" //random number generator header
#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/masterSyllableListTree.h"
#include "../../headers/masterSyllableListMap.h"
#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"
#include "../../headers/badwordContainer.h"
#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

#include "../../objects/windowContainer.cpp"

using namespace std;
#include "MSLUtils.h"

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	//masterSyllablesListTree physicalMSL;
    extern masterSyllablesListMap physicalMSL;
	extern masterSyllablesList* MSL; //MasterSyllablesList
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern hLog histLog;
	extern time_t sessionStartTime;
}

void createRandomWordWrongCounts();
void printVector(const vector<string>& sV, int start=0, int stop=-1);
void printVector(const vector<int>& sV, int start=0, int stop=-1);

bool MSLSortingTest(masterSyllablesList* MSL)
{
	MSL->sortList();
	//MSL->print(); //For manual inspection
	vector<string> syllablePos = {"HD","AH0","ZH","BN"};
	vector<int> syllableValue = {5,3,1,0};
	return validateMSLState(MSL,syllablePos,syllableValue);
}

void runMSLSortingTests()
{
	vector<string> initialState = {"AH0", "ZH", "BN", "HD"};
	vector<int> syllableWCount = {3,1,0,5};
	masterSyllablesListTree* MSLTree = new masterSyllablesListTree();
	initMSL(initialState, syllableWCount, MSLTree);
	if (MSLSortingTest(MSLTree))
		cout << "MSLTree sorting [passed]\n";
	else
		cout << "MSLTree sorting [failed]\n";
	delete MSLTree;

	masterSyllablesListMap* MSLMap = new masterSyllablesListMap();
	initMSL(initialState, syllableWCount, MSLMap);
	if (MSLSortingTest(MSLMap))
		cout << "MSLMap sorting [passed]" << endl;
	else
		cout << "MSLMap sorting [failed]" << endl;
		delete MSLMap;
}

bool testMSLMap(const string& testType, masterSyllablesList* MSL)
{
	for (int i=0; i<MSL->size(); i++) //For every syllable in the masterSyllableList
	{
		string testSyllable = (*MSL)[i];
		//cout << SSG::MSL.getSyllableWCount(i) << "  " << SSG::MSL.wrongCount[i] << '\n';
		//cout << "Testing syllable " << testSyllable << " at position " << i << " does " << SSG::MSL.getSyllableWCount(i) << "==" << SSG::MSL.getSyllableWCount(testSyllable) << "?\n";
		if (MSL->getSyllableWCount(i) != MSL->getSyllableWCount(testSyllable)) //If there is a mismatch in the returned wrongCount
		{
			cout << "Error MSL position and value do mismatch!\nTest syllable " << testSyllable << " at position " << i << endl;
			cout << testType << "[Failed]" << endl;
			return false;
		}
	}
	cout << testType << "[Passed]" << endl;
}
