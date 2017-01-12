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
void printVector(const vector<int>& sV, int start=0, int stop=-1);

void runMSLSortingTests();
void runMSLaddToTotalTest();
void runtestMSLoverridedSWC();
void runCompleteMSLADDSyllables();
bool testWordContainerSearch(wordContainer& goodWords);
void runTestWordContainerStringCompare(wordContainer& goodWords);
void runAllhLogTests();

// class hLogTester : public hLog {
// public:
// 	hLogTester(const std::string& saveLocation) : hLog(saveLocation) {;}
//
// 	bool testGetEventPointers(std::time_t startTime, const string& eventType, int expected)
// 	{
// 		vector<logEvent*> testEvents;
// 		getEventPointers(startTime,testEvents,eventType);
// 		if (testEvents.size() != expected)
// 			return false;
// 		return true;
// 	}
//
// 	bool findTimeStartTest(vector<std::time_t> startTimes, vector<int> pos)
// 	{
// 		for (int i=0; i<startTimes.size(); ++i)
// 		{
// 			if (findTimeStart(startTimes[i]) != pos[i])
// 				return false;
// 		}
// 		return true;
// 	}
//
// 	int size() const {return log.size();}
//
// 	bool findtotalWREPlaytimeTest(vector<std::time_t> startTimes, vector<int> totals)
// 	{
// 		for (int i=0; i<startTimes.size(); ++i)
// 		{
// 			vector<logEvent*> testEvents;
// 			getEventPointers(startTimes[i],testEvents,"HangmanComplete");
// 			if (findTotalWREPlaytime(testEvents) != totals[i])
// 			{
// 				cout << "Test failed on test# " << i+1 << " total time spent " << findTotalWREPlaytime(testEvents) << endl;
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
// };
//
// void runfindTimeStartTest()
// {
// 	hLogTester tester("./Data/SSGHistFile.txt");
// 	cout << "hLog find Time Start Test ";
// 	if (tester.findTimeStartTest(
// 		{0x58580300,
// 		 0x0,
// 	 	 time(0)},
// 		{4,0,tester.size()}))
// 		cout << "[Passed]" << endl;
// 	else
// 		cout << "[Failed]" << endl;
// }
//
// void runfindtotalWREPlaytimeTest()
// {
// 	hLogTester tester("./Data/SSGHistFile.txt");
// 	if (tester.findtotalWREPlaytimeTest(
// 		{0x0,
// 		 time(0),
// 	 	 0x58664383},
// 		{212,0,153}))
// 		cout << "hLog find total WRE Playtime Test [Passed]" << endl;
// 	else
// 		cout << "hLog find total WRE Playtime Test [Failed]" << endl;
// }
//
// void runHLogGetEventPointersTests()
// {
// 	hLogTester tester("./Data/SSGHistFile.txt");
// 	if (tester.testGetEventPointers(0x58665807,"HangmanComplete",10))
// 		cout << "hLog getEventPointers 1 [Passed]\n";
// 	else
// 		cout << "hLog getEventPointers 1 [Failed]\n";
//
// 	if (tester.testGetEventPointers(time(0),"HangmanComplete",0))
// 		cout << "hLog getEventPointers 2 [Passed]\n";
// 	else
// 		cout << "hLog getEventPointers 2 [Failed]\n";
//
// 	if (tester.testGetEventPointers(0x0,"DoesNotExist",0))
// 		cout << "hLog getEventPointers 3 [Passed]\n";
// 	else
// 		cout << "hLog getEventPointers 3 [Failed]\n";
// }

void logEventGenEventLineTest()
{
	string EventLine = "585a9fe5+GoodWordWrong+ABACK+3.863636+abba";
	logEvent lE(EventLine);
	if (lE.generateEventLine() == EventLine)
		cout << "logEvent eventLine from eventLine test [Passed]" << endl;
	else
		cout << "logEvent eventLine from eventLine test [Failed]" << endl;

	logEvent lETwo({"ABACK","3.863636","abba"},0x585a9fe5,"GoodWordWrong");
	if (lETwo.generateEventLine() == EventLine)
		cout << "logEvent eventLine from dataItems test [Passed]" << endl;
	else
		cout << "logEvent eventLine from dataItems test [Failed]" << endl;
}

int main(int argc, char const *argv[]) {
    wordContainer goodWords("./Data/finalDictwithDef.txt");
    //wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt
	testWordContainerSearch(goodWords);
	runTestWordContainerStringCompare(goodWords);
	createRandomWordWrongCounts();
	runMSLSortingTests();
	runMSLaddToTotalTest();
	runtestMSLoverridedSWC();
	runCompleteMSLADDSyllables();
	logEventGenEventLineTest();
	runAllhLogTests();

	cout << "Tests complete" << endl;
    return 0;
}
