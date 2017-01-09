#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

//#include "../headers/SSG.h"
#include "../headers/word.h"
#include "../headers/hLog.h"
#include "../headers/masterSyllableList.h"

#include "math.h"
#include <complex>

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
	extern hLog histLog;
}

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);

//protected
int word::determineSyllables()
{
	float boundryAmount = wordC.size()/syllables.size();
	float currentBoundry = 0;
	int boundryAmountInt = (int)boundryAmount;
	int currentBoundryInt = 0;

	for (int i=0; i<syllables.size();i++)
	{
		currentBoundryInt = (int)currentBoundry;
		syllablePositions.push_back(currentBoundryInt);
		//cout << wordC.substr(currentBoundryInt,boundryAmountInt) << ":" << syllables[i] << endl;
		//cout << currentBoundry << ":" << currentBoundryInt << endl;
		//cout << boundryAmount << ":" << boundryAmountInt << endl;
		currentBoundry += boundryAmount;
	}
}

void word::addToMSLTotal(const vector<int>& syllableWrongCount) const
{
    for (int i=0; i<syllableWrongCount.size(); i++)
    {
        //cout << syllables[i] << "wrong " << syllableWrongCount[i] << endl;
        SSG::MSL->addToTotal(syllables[i],syllableWrongCount[i]);
    }
}

//public
word::word(bool safteyMechanism)
{
	cout << "WARNING DEFAULT CONSTRUCTER BEING CALLED, OBJECT *MUST* ALREADY BE CONSTRUCTED!" << endl;
}

word::word(word* bwordToSlice)
{
	//This function intenionally slices a badWord back to a word;
	wordC=bwordToSlice->getWord();
	syllables=bwordToSlice->getSyllables();
	syllablePositions=bwordToSlice->getSyllablePositions();
	definition=bwordToSlice->getDefinition();
	wordFlags=bwordToSlice->getWordFlags();
	wScore=bwordToSlice->getwScore();
	weight=0.5;
}

word::word(const string& wordline)
{
	//cout << wordline << endl;
	vector<string> wordVec(split(wordline));
	//cout << wordline.size() << endl;
	//size = wordVec[0].size();
	wordC = wordVec[0]; //set wordCapitalised to the word
	//cout << "Creating " << wordC << endl;

	//insert code to add flags
	for (unsigned int i=1; i < wordVec.size(); i++) //Update i start value when flags added
	{
		//cout << "Checking " << wordVec[i] << endl;
		if (wordVec[i] == "#DEF")
		{
			//cout << wordVec[i] << wordVec[i+1] << endl;
			definition = wordVec[i+1]; //if current string is #DEF the next string must be the definition
			break;
		}
		//Can now assume not a flag or a definition
		syllables.push_back(wordVec[i]);
	}

	//Check the syllables exist in MSB

	//cout << definition << endl;

	//printVector(syllables);

	SSG::MSL->addSyllables(syllables); //Add the syllables to the MSL, they will only be added if not added before.
	determineSyllables();

	for (unsigned int i=0; i<8; i++) //Set all flags to 0
	{
		wordFlags.push_back(false);
	}

}

word::~word()
{
	;//All destruction occurs when destorying member objects, virtual destructor needed for correct destruction of badWord
}

bool word::wordCorrect()
{
	weight -= 0.1;
	if (weight<0)
		weight=0;
	return false; //Needed for check of badword
}

float word::getwScore() const {return wScore;}

string word::getWord() const {return wordC;}

float word::getWeight() const {return weight;}

std::vector<std::string> word::getSyllables() const {return syllables;}

std::vector<int> word::getSyllablePositions() const {return syllablePositions;}

std::string word::getDefinition() const {return definition;}

std::vector<bool> word::getWordFlags() const {return wordFlags;}
