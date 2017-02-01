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

class wordCCTester : public wordCC {

vector<string> preTestState;
public:

    wordCCTester(std::string goodFilename, std::string badFilename) : wordCC(goodFilename,badFilename)
    {cout << "Running wordCC Tests" << endl;}

    bool verifyWordCCState()
    {
        for (int i=0, wordCCSize=size(); i<wordCCSize; ++i)
        {
            if (preTestState[i] != getWord(i)->getWord())
                return false;
        }
        return true;
    }

    void determineCurrentState()
    {
        preTestState.clear();
        for (int i=0, wordCCSize=size(); i<wordCCSize; ++i)
        {
            preTestState.push_back(getWord(i)->getWord());
        }
    }

    int determineVulcanLocation()
    {
        for (int i=0, wordCCSize=size(); i<wordCCSize; ++i)
        {
            if (getWord(i)->getWord() == "VULCAN")
                {return i;}
        }
        return -1;
    }

    bool badWordCorrectTest()
    {
        determineCurrentState();
        int vulcanLoc=determineVulcanLocation();
        int maxIter;
        if (vulcanLoc > -1)
        {
            while (maxIter < 20)
            {
                wordCorrect(vulcanLoc);
                if (goodBadPos[vulcanLoc] == false) //If now a goodWord
                    break;
                ++maxIter;
            }
            if (maxIter == 20)
                return false;
        }

        return verifyWordCCState();
    }

    bool goodWordWrongTest()
    {
        determineCurrentState();
        int vulcanLoc=determineVulcanLocation();

        if (vulcanLoc > -1)
        {
            wordWrong(vulcanLoc, "VLUCAN");

            if (goodBadPos[vulcanLoc] == false)//If still a goodWord
                return false;
        }

        return verifyWordCCState();
    }

    void runKeyboardWordsTest()
    {
        findKeyboardWords();
        int badWordCount = 0;
        int goodWordCount = 0;

        for (int i=0; i<200; ++i)
        {
            if (getWord(i)->getWordFlags()[0] == true)
                ++badWordCount;
            else
                ++goodWordCount;
        }

        cout << "Badwords: " << badWordCount << " GoodWords: " << goodWordCount << endl;
        if (goodWordCount >= 30)
            cout << "Keyboard Words 30 Words Test [Passed]" << endl;
        else
            cerr << "Keyboard Words 30 Words Test [Failed]" << endl;
    }
};

void runSpellingOverFlowTest(wordCCTester& CCTester)
{
    CCTester.findSpellingWords();
    for (int i=0, end=CCTester.size(); i<end; ++i)
    {
        CCTester.nextWord();
    }
    if (CCTester.getCurrentPosition() == 0)
    {
        if (SSG::histLog[SSG::histLog.size()-1].getType() == "SpellingOverFlow")
            cout << "SpellingOverFlow test [Passed]" << endl;
        else
            cerr << "SpellingOverFlow test Type wrong [Failed]" << endl;
    }
    else
        {cerr << "SpellingOverFlow test CurrentWord wrong [Failed]" << endl; cerr << "Current word: " << CCTester.getCurrentPosition() << endl;}
}

void runWrongCorrectTests(wordCCTester& CCTester)
{
    //hLog oldLog = SSG::histLog;
    //SSG::histLog = hLog("./Data/TestsDebugLog.txt");

    // wordCCTester CCTester("./Data/shortDict.txt", "./Data/shortwrongWords.txt");
    CCTester.findSpellingWords();
    if (CCTester.badWordCorrectTest())
        cout << "WordCC Bad Word Correct Test [Passed]" << endl;
    else
        cout << "WordCC Bad Word Correct Test [Failed]" << endl;

    if (CCTester.goodWordWrongTest())
        cout << "WordCC Good Word Wrong Test [Passed]" << endl;
    else
        cout << "WordCC Good Word Wrong Test [Failed]" << endl;

    //SSG::histLog = oldLog;
}

void runWordCCTests()
{
    wordCCTester CCTester("./Data/shortDict.txt", "./Data/shortwrongWords.txt");
    runWrongCorrectTests(CCTester);
    runSpellingOverFlowTest(CCTester);
    wordCCTester CCTesterLarge("./Data/finalDictwithDef.txt", "./Data/wrongWords.txt");
    CCTesterLarge.runKeyboardWordsTest();
}
