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

class wordTester : public word {
public:
	wordTester(const string& wordline) : word(wordline) {;}
	bool DetermineWrongSyllablesTest(const vector<string>& spellings, const vector<vector<int>> expectedResults)
	{
		for (int i=0; i<spellings.size(); ++i)
		{
			if (determineWrongSyllables(spellings[i]) != expectedResults[i])
				return false;
			//printVector(determineWrongSyllables(spellings[i])); //Uncomment to enable console printing
		}
		return true;
	}
};

void runDetermineWrongSyllablesTest()
{
	wordTester tester1("ABBE+AE1+B+IY0+#DEF+Unimportant");
	if (tester1.DetermineWrongSyllablesTest(
		{"AWBE","wBBE","ABBW"},
		{{0,1,0},{1,0,0},{0,0,1}}))
		cout << "Word Determine Wrong Syllables Test 1 [Passed]" << endl;
	else
		cout << "Word Determine Wrong Syllables Test 1 [Failed]" << endl;
	wordTester tester2("ABDICATE+AE1+B+D+AH0+K+EY2+T+#DEF+Unimportant");
	if (tester2.DetermineWrongSyllablesTest(
		{"ABDICATE","ABDIXATE","ABDICAT","ABDICATEX"},
		{{0,0,0,0,0,0,0},{0,0,0,0,1,0,0},{0,0,0,0,0,0,1},{0,0,0,0,0,0,1}}))
		cout << "Word Determine Wrong Syllables Test 2 [Passed]" << endl;
	else
		cout << "Word Determine Wrong Syllables Test 2 [Failed]" << endl;
}

void runAllWordTests()
{
    runDetermineWrongSyllablesTest();
}
