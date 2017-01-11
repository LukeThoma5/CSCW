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

void testMSLoverridedSWC(masterSyllablesList* MSL, const string& testString)
{
	vector<string> syllables = {"AH0","ZH","LF"};
	vector<int> expectedSWC = {5,7,2};
	initMSL(syllables,expectedSWC,MSL);
	for (int j=0; j<2; ++j)
	{
		vector<int> syllWC;
		syllWC.reserve(3);
		for (int i=0; i<3; ++i)
		{
			if (j)
				syllWC.push_back(MSL->getSyllableWCount(syllables[i]));
			else
				syllWC.push_back(MSL->getSyllableWCount(i));
		}
		//printVector(syllWC); //Uncomment for manual inspection
		if (syllWC == expectedSWC)
			cout << testString << j+1 << " [Passed]" << endl;
		else
			cout << testString << j+1 << " [Failed]" << endl;
	}
}

void runtestMSLoverridedSWC()
{
	masterSyllablesListTree* MSLTree = new masterSyllablesListTree();
	testMSLoverridedSWC(MSLTree,"MSLTree MSL Overrided SWC ");
	delete MSLTree;
	masterSyllablesListMap* MSLMap = new masterSyllablesListMap();
	testMSLoverridedSWC(MSLMap,"MSLMap MSL Overrided SWC ");
	delete MSLMap;
}
