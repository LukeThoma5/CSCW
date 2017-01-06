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

bool testWordContainerSearch(wordContainer& goodWords)
{
    //string.compare has issues when passed "antic" and "anti-Federalist", doesn't like the -
    //bool success = true;
    int j = 0;
    for (int i=0; i<goodWords.size(); i++)
    {
        //cout << "### Now searching for " << goodWords[i]->getWord() << endl;
        int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
        if (location == -1)
        {
            cout << "Word not found" << endl;
            return false;
        }
        if (i != location)
        {
            cout << "Binary Search Test [Failed]" << endl;
            cout << "An error has occured " << i << " != " << location << endl;
            return false;
        }
        j++;
        if (j > 1000)
        {
            int location = goodWords.binSearch(goodWords[i]->getWord(),0,goodWords.size());
            //cout << "Looking for word # " << i << " with value " << goodWords[i]->getWord() << " which was found at location " << location << " with a value of " << goodWords[location]->getWord() << endl;
            j=0;
        }
    }
    //cout << "Binary Search Test [Passed]" << endl;
    return true;
}

bool testMSLMap(const string& testType)
{
	for (int i=0; i<SSG::MSL->size(); i++) //For every syllable in the masterSyllableList
	{
		string testSyllable = (*SSG::MSL)[i];
		//cout << SSG::MSL.getSyllableWCount(i) << "  " << SSG::MSL.wrongCount[i] << '\n';
		//cout << "Testing syllable " << testSyllable << " at position " << i << " does " << SSG::MSL.getSyllableWCount(i) << "==" << SSG::MSL.getSyllableWCount(testSyllable) << "?\n";
		if (SSG::MSL->getSyllableWCount(i) != SSG::MSL->getSyllableWCount(testSyllable)) //If there is a mismatch in the returned wrongCount
		{
			cout << "Error MSL position and value do mismatch!\nTest syllable " << testSyllable << " at position " << i << endl;
			cout << testType << "[Failed]" << endl;
			return false;
		}
	}
	cout << testType << "[Passed]" << endl;
}

void printMSL()
{
	for (int i=0; i<SSG::MSL->size(); i++)
	{
		cout << (*SSG::MSL)[i] << '[' << SSG::MSL->getSyllableWCount(i) << " :: " << SSG::MSL->getSyllableWCount((*SSG::MSL)[i]) << "]\n";
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

int main(int argc, char const *argv[]) {
    wordContainer goodWords("finalDictwithDef.txt");
    //wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt
	testWordContainerSearch(goodWords);
	createRandomWordWrongCounts();
	printMSL();
	testMSLMap("MSL Map Creation Test ");
	SSG::MSL->sortList();
	testMSLMap("Merge sorted Map Test ");
	printMSL();

	runCompleteMSLADDSyllables();

	cout << "Tests complete" << endl;
    return 0;
}
