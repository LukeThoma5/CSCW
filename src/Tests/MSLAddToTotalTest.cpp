#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/masterSyllableListTree.h"
#include "../../headers/masterSyllableListMap.h"

using namespace std;
#include "MSLUtils.h"

void MSLaddToTotalTest(masterSyllablesList* MSL, const string& testString, bool isFirstTest)
{
	bool hasPassed;
	if (isFirstTest)
	{
		initMSL({"AH0"},{5},MSL);
		MSL->addToTotal("AH0",1);
		hasPassed = validateMSLState(MSL,{"AH0"},{6});
		if (hasPassed)
		{
			MSL->addToTotal("ZH",3);
			hasPassed = validateMSLState(MSL,{"AH0","ZH"},{6,3});
			if (hasPassed)
			{
				MSL->addToTotal("ZH",2);
				hasPassed = validateMSLState(MSL,{"AH0","ZH"},{6,5});
			}
		}
		MSL->print();
	}
	else
	{
		//MSL is empty
		MSL->print();
		MSL->addToTotal("ZH",3);
		MSL->print();
		hasPassed = validateMSLState(MSL,{"ZH"},{3});
	}

	if (hasPassed)
		cout << testString << "Passed]" << endl;
	else
		cout << testString << "Failed]" << endl;
}

void runMSLaddToTotalTest()
{
	masterSyllablesListTree* MSLTree = new masterSyllablesListTree();
	MSLaddToTotalTest(MSLTree, "MSLTree addToTotalTest 1 [",true);
	delete MSLTree; MSLTree = new masterSyllablesListTree();
	MSLaddToTotalTest(MSLTree, "MSLTree addToTotalTest 2 [",false);
	delete MSLTree;

	masterSyllablesListMap* MSLMap = new masterSyllablesListMap();
	MSLaddToTotalTest(MSLMap, "MSLMap addToTotalTest 1 [",true);
	delete MSLMap; MSLMap = new masterSyllablesListMap();
	MSLaddToTotalTest(MSLMap, "MSLMap addToTotalTest 2 [",false);
	delete MSLMap;
}
