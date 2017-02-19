#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/masterSyllableListTree.h"
#include "../../headers/masterSyllableListMap.h"

using namespace std;
#include "MSLUtils.h"

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
