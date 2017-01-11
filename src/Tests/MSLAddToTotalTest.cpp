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

void MSLaddToTotalTest(masterSyllablesList* MSL, const string& testString, bool isFirstTest)
{
	bool hasPassed;
	if (isFirstTest)
	{
		initMSL({"AH0"},{5},MSL);
		MSL->addToTotal("AH0",1);
		hasPassed = validateMSLState(MSL,{"AH0"},{6});
		if (hasPassed)
		{
			MSL->addToTotal("ZH",3);
			hasPassed = validateMSLState(MSL,{"AH0","ZH"},{6,3});
			if (hasPassed)
			{
				MSL->addToTotal("ZH",2);
				hasPassed = validateMSLState(MSL,{"AH0","ZH"},{6,5});
			}
		}
		MSL->print();
	}
	else
	{
		//MSL is empty
		MSL->print();
		MSL->addToTotal("ZH",3);
		MSL->print();
		hasPassed = validateMSLState(MSL,{"ZH"},{3});
	}

	if (hasPassed)
		cout << testString << "Passed]" << endl;
	else
		cout << testString << "Failed]" << endl;
}

void runMSLaddToTotalTest()
{
	masterSyllablesListTree* MSLTree = new masterSyllablesListTree();
	MSLaddToTotalTest(MSLTree, "MSLTree addToTotalTest 1 [",true);
	delete MSLTree; MSLTree = new masterSyllablesListTree();
	MSLaddToTotalTest(MSLTree, "MSLTree addToTotalTest 2 [",false);
	delete MSLTree;

	masterSyllablesListMap* MSLMap = new masterSyllablesListMap();
	MSLaddToTotalTest(MSLMap, "MSLMap addToTotalTest 1 [",true);
	delete MSLMap; MSLMap = new masterSyllablesListMap();
	MSLaddToTotalTest(MSLMap, "MSLMap addToTotalTest 2 [",false);
	delete MSLMap;
}
