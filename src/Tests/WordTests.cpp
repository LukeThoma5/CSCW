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

    bool generateBadWordLineTest(const vector<int>& syllableWrongCount, const string& expectedResult)
    {
        if (generateBadWordLine(syllableWrongCount) != expectedResult)
        {
            cout << generateBadWordLine(syllableWrongCount) << " vs " << expectedResult << endl;
            return false;
        }
        return true;
    }

    void addToMSLTotalTest(const vector<int>& syllableWrongCount)
    {
        addToMSLTotal(syllableWrongCount);
    }
};

void runGenBadWordLineTest(wordTester& tester1, wordTester& tester2)
{
    bool hasPassed = true;
    if (!tester1.generateBadWordLineTest({1,2,3},"ABBE+1.0+1+2+3"))
        hasPassed = false;
    if (!tester1.generateBadWordLineTest({-3,-2,-1},"ABBE+1.0+-3+-2+-1"))
        hasPassed = false;
    if (!tester2.generateBadWordLineTest({0,0,0,0,0,0,0},"ABDICATE+1.0+0+0+0+0+0+0+0"))
        hasPassed = false;
    if (!tester2.generateBadWordLineTest({-7,-6,-5,-4,-3,-2,-1},"ABDICATE+1.0+-7+-6+-5+-4+-3+-2+-1"))
        hasPassed = false;
    if (hasPassed)
        cout << "Word Generate Bad Word Line Test [Passed]" << endl;
    else
        cout << "Word Generate Bad Word Line Test [Failed]" << endl;
}

void runDetermineWrongSyllablesTest(wordTester& tester1, wordTester& tester2)
{
	if (tester1.DetermineWrongSyllablesTest(
		{"AWBE","wBBE","ABBW"},
		{{0,1,0},{1,0,0},{0,0,1}}))
		cout << "Word Determine Wrong Syllables Test 1 [Passed]" << endl;
	else
		cout << "Word Determine Wrong Syllables Test 1 [Failed]" << endl;
	if (tester2.DetermineWrongSyllablesTest(
		{"ABDICATE","ABDIXATE","ABDICAT","ABDICATEX"},
		{{0,0,0,0,0,0,0},{0,0,0,0,1,0,0},{0,0,0,0,0,0,1},{0,0,0,0,0,0,1}}))
		cout << "Word Determine Wrong Syllables Test 2 [Passed]" << endl;
	else
		cout << "Word Determine Wrong Syllables Test 2 [Failed]" << endl;
}

void runaddToMSLTotalTest(wordTester& tester)
{
    //Set SSG::MSL to a fresh one so that we can easily verify it worked
    masterSyllablesListTree MSLTree;
    masterSyllablesList* OLDSSGMSL = SSG::MSL;
    SSG::MSL = &MSLTree;

    //Initialise the MSL to the wanted state
    initMSL({"AE1","B","D","AH0","K","EY2","T","OTHER"},
            {1,2,3,4,5,6,7,8}, &MSLTree);
    //Make tester run protected function
    tester.addToMSLTotalTest({10,20,30,40,50,60,70});

    //If the MSL has the expected values, print appropriately
    if (validateMSLState(&MSLTree, {"AE1","B","D","AH0","K","EY2","T","OTHER"}, {11,22,33,44,55,66,77,8}))
        cout << "Word Add To MSL Total Test [Passed]" << endl;
    else
        cout << "Word Add To MSL Total Test [Failed]" << endl;

    SSG::MSL = OLDSSGMSL; //Reset the MSL to what it was before MSLTree goes out of scope.
}

void runWordScoreTest()
{
    //Set SSG::MSL to a fresh one so that we can easily verify it worked
    masterSyllablesListTree MSLTree;
    masterSyllablesList* OLDSSGMSL = SSG::MSL;
    SSG::MSL = &MSLTree;

    //Initialise the MSL to the wanted state
    //S:1, N:2, AE1:3, P:4, IY0:5
    initMSL({"S","N","AE1","P","IY0"},
            {1,2,3,4,5}, &MSLTree);

    string snappyConstructor = "SNAPPY+S+N+AE1+P+IY0+#DEF+Snappish.";
    word goodSnappy(snappyConstructor);

    goodSnappy.determineScore();
    float gSwS = goodSnappy.getwScore();
    if (gSwS=float(1.431818))
        cout << "Good Word Determine Score Test [Passed]" << endl;
    else
    {
        cout << "Good Word Determine Score Test [Failed]" << endl;
        cout << fixed << double(goodSnappy.getwScore()) << endl;
    }

    badWord badSnappy("SNAPPY+S+N+AE1+P+IY0+#DEF+Snappish.", "SNAPPY+2.0+0+1+2+3+4");
    badSnappy.determineScore();

    if (badSnappy.getwScore()==float(9.545455))
        cout << "Bad Word Determine Score Test [Passed]" << endl;
    else
        {
            cout << "Bad Word Determine Score Test [Failed]" << endl;
            cout << fixed << double(badSnappy.getwScore()) << endl;
        }

    SSG::MSL = OLDSSGMSL; //Reset the MSL to what it was before MSLTree goes out of scope.
}

void runWordScoreAttributeTest()
{
    masterSyllablesListTree MSLTree;
    masterSyllablesList* OLDSSGMSL = SSG::MSL;
    SSG::MSL = &MSLTree;

    initMSL({"AH0","F","ER1","M","TSYLLABLE"},{1,2,3,4,11},&MSLTree);
    wordTester tester("AFFIRM+AH0+F+ER1+M+#DEF+shortDef");
    tester.determineScore();
    float originalTesterScore = tester.getwScore();
    MSLTree.addToTotal("AH0",1); tester.determineScore();
    float secondayTesterScore = tester.getwScore();

    if (secondayTesterScore > originalTesterScore)
        cout << "Word Score attribute test 1 [Passed]" << endl;
    else
        cerr << "Word Score attribute test 1 [Failed]" << endl;

    wordTester tester2("TEWORD+TSYLLABLE+#DEF+shortDEF");
    tester2.determineScore();

    if (tester2.getwScore() > secondayTesterScore)
        cout << "Word Score attribute test 2 [Passed]" << endl;
    else
        cerr << "Word Score attribute test 2 [Failed]" << endl;

    wordTester tester3("AFFIR+AH0+F+ER1+M+#DEF+shortDef");
    tester3.determineScore();
    if (tester3.getwScore() < secondayTesterScore)
        cout << "Word Score attribute test 3 [Passed]" << endl;
    else
        cerr << "Word Score attribute test 3 [Failed]" << endl;

    badWord bTester("TEWORD+TSYLLABLE+#DEF+shortDEF","TEWORD+1.0+0");
    bTester.determineScore();
    float originalBTesterScore = bTester.getwScore();
    bTester.wordCorrect(); bTester.determineScore();
    if (bTester.getwScore() < originalBTesterScore)
        cout << "Word Score attribute test 4 [Passed]" << endl;
    else
        cerr << "Word Score attribute test 4 [Failed]" << endl;

    bTester.wordWrong("TEWORD"); //Completely correct spelling will mean that only the weight will change not the MSL values
    bTester.determineScore();
    if (bTester.getwScore() > originalBTesterScore)
        cout << "Word Score attribute test 5 [Passed]" << endl;
    else
        cerr << "Word Score attribute test 5 [Failed]" << endl;

    cout << originalTesterScore << " " << secondayTesterScore \
    << " " << tester2.getwScore() << " " << tester3.getwScore() \
    << " " << originalBTesterScore << endl;

    SSG::MSL = OLDSSGMSL; //Reset the MSL to what it was before MSLTree goes out of scope.
}

void runAllWordTests()
{
    wordTester tester1("ABBE+AE1+B+IY0+#DEF+Unimportant");
    wordTester tester2("ABDICATE+AE1+B+D+AH0+K+EY2+T+#DEF+Unimportant");
    runDetermineWrongSyllablesTest(tester1, tester2);
    runGenBadWordLineTest(tester1, tester2);
    runaddToMSLTotalTest(tester2);
    runWordScoreTest();
    runWordScoreAttributeTest();
}
