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

bool testMSLAddSyllables(masterSyllablesList* MSL, const vector<string>& initialState, const vector<vector<string>>& testData)
{
	cout << "testingMSLAddSyllables" << endl;
	if (!MSL->size())
	{
		MSL->addSyllables(initialState);
		cout << "MSL initialed" << endl;
		MSL->print();
		for (int i=0; i<testData.size(); i++)
		{
			cout << "Adding vector with value";
			printVector(testData[i]);
			MSL->addSyllables(testData[i]);
			MSL->print();
		}
		bool  success = verifyMSLSingleInstance(mergeAllDataToOneVector(initialState,testData),MSL);
		cout << "testMSLAddSyllables complete with value " << success << endl;
		return success;

	}
	else
	{
		cout << "Test failed, non empty MSL passed to testMSLAddSyllables" << endl;
	}
}

void runCompleteMSLADDSyllables()
{
	masterSyllablesListTree* MSLTree = new masterSyllablesListTree();
	testMSLAddSyllables(MSLTree,{"AH0","ZH"},{{"AH0","BA"}, {"zh","HE"}});
	delete MSLTree; MSLTree = new masterSyllablesListTree();
	testMSLAddSyllables(MSLTree,{},{ {"AH0"}, {} });
	delete MSLTree;

	masterSyllablesListMap* MSLMap = new masterSyllablesListMap();
	testMSLAddSyllables(MSLMap,{"AH0","ZH"},{{"AH0","BA"}, {"zh","HE"}});
	delete MSLMap; MSLMap = new masterSyllablesListMap();
	testMSLAddSyllables(MSLMap,{},{ {"AH0"}, {} });
	delete MSLMap;
}
