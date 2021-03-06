#include "../../headers/hLog.h"

using namespace std;

class hLogTester : public hLog {
public:
	hLogTester(const std::string& saveLocation) : hLog(saveLocation) {;}

	bool testGetEventPointers(std::time_t startTime, const string& eventType, int expected)
	{
		vector<logEvent*> testEvents;
		getEventPointers(startTime,testEvents,eventType);
		if (testEvents.size() != expected)
			return false;
		return true;
	}

	bool findTimeStartTest(vector<std::time_t> startTimes, vector<int> pos)
	{
		for (int i=0; i<startTimes.size(); ++i)
		{
			if (findTimeStart(startTimes[i]) != pos[i])
				return false;
		}
		return true;
	}

	int size() const {return log.size();}

	bool findtotalWREPlaytimeTest(vector<std::time_t> startTimes, vector<int> totals)
	{
		for (int i=0; i<startTimes.size(); ++i)
		{
			vector<logEvent*> testEvents;
			getEventPointers(startTimes[i],testEvents,"HangmanComplete");
			if (findTotalWREPlaytime(testEvents) != totals[i])
			{
				cout << "Test failed on test# " << i+1 << " total time spent " << findTotalWREPlaytime(testEvents) << endl;
				return false;
			}
		}
		return true;
	}
};

void runfindTimeStartTest(hLogTester& tester)
{
	cout << "hLog find Time Start Test ";
	if (tester.findTimeStartTest(
		{0x58580300,
		 0x0,
	 	 time(0)},
		{4,0,tester.size()}))
		cout << "[Passed]" << endl;
	else
		cout << "[Failed]" << endl;
}

void runfindtotalWREPlaytimeTest(hLogTester& tester)
{
	if (tester.findtotalWREPlaytimeTest(
		{0x0,
		 time(0),
	 	 0x58664383},
		{212,0,153}))
		cout << "hLog find total WRE Playtime Test [Passed]" << endl;
	else
		cout << "hLog find total WRE Playtime Test [Failed]" << endl;
}

void runHLogGetEventPointersTests(hLogTester& tester)
{
	if (tester.testGetEventPointers(0x58665807,"HangmanComplete",10))
		cout << "hLog getEventPointers 1 [Passed]\n";
	else
		cout << "hLog getEventPointers 1 [Failed]\n";

	if (tester.testGetEventPointers(time(0),"HangmanComplete",0))
		cout << "hLog getEventPointers 2 [Passed]\n";
	else
		cout << "hLog getEventPointers 2 [Failed]\n";

    if (tester.testGetEventPointers(0x0,"HangmanComplete",14))
		cout << "hLog getEventPointers 3 [Passed]\n";
	else
		cout << "hLog getEventPointers 3 [Failed]\n";

	if (tester.testGetEventPointers(0x0,"DoesNotExist",0))
		cout << "hLog getEventPointers 4 [Passed]\n";
	else
		cout << "hLog getEventPointers 4 [Failed]\n";
}

void runAllhLogTests()
{
    hLogTester tester("./Data/SSGHistDebug.txt");
    runfindTimeStartTest(tester);
    runfindtotalWREPlaytimeTest(tester);
    runHLogGetEventPointersTests(tester);
}
