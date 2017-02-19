#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/masterSyllableListTree.h"
#include "../../headers/masterSyllableListMap.h"

using namespace std;

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
}

#include "MSLUtils.h"

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
