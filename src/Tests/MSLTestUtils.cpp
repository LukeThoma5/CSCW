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

void initMSL(const vector<string>& initialState, const vector<int>& syllableWCount, masterSyllablesList* MSL)
{
	MSL->addSyllables(initialState);
	for (int i=0; i<MSL->size(); ++i)
		MSL->addToTotal(initialState[i], syllableWCount[i]);
}

bool validateMSLState(masterSyllablesList* MSL, const vector<string>& syllablePos, const vector<int>& syllableValue)
{
	if (MSL->size() != syllablePos.size() || MSL->size() != syllableValue.size())
		return false;
	for (int i=0; i<MSL->size(); i++)
	{
		if ((*MSL)[i] != syllablePos[i] || MSL->getSyllableWCount(i) != syllableValue[i] || MSL->getSyllableWCount(syllablePos[i]) != syllableValue[i])
			return false;
	}
	return true;
}

void printMSL()
{
	for (int i=0; i<SSG::MSL->size(); i++)
	{
		cout << (*SSG::MSL)[i] << '[' << SSG::MSL->getSyllableWCount(i) << " :: " << SSG::MSL->getSyllableWCount((*SSG::MSL)[i]) << "]\n";
	}
}

bool verifyMSLSingleInstance(const vector<string>& syllables, masterSyllablesList* MSL)
{
	for (int i=0; i<syllables.size(); i++)
	{
		int occurences = 0;
		for (int j=0; j<MSL->size(); j++)
		{
			if ((*MSL)[j] == syllables[i])
				++occurences;
		}
		if (occurences != 1)
			return false;
	}
	return true;
}

vector<string> mergeAllDataToOneVector(const vector<string>& initialState, const vector<vector<string>>& testData)
{
	vector<string> dumpLocation = initialState;
	int totalSize = initialState.size();
	for (int i=0; i<testData.size(); i++)
	{
		totalSize += testData[i].size();
	}
	dumpLocation.reserve(totalSize); //Reduce Mem Allocs

	for (int i=0; i<testData.size(); i++)
	{
		for (int j=0; j<testData[i].size(); j++)
		{
			dumpLocation.push_back(testData[i][j]);
		}
	}
	return dumpLocation;
}
