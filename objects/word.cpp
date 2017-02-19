#include "../headers/word.h"
#include "../headers/hLog.h"
#include "../headers/masterSyllableList.h"

#include <complex>

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
	extern hLog histLog;
}

using namespace std;

//protected
int word::determineSyllables()
{
	float boundryAmount = wordC.size()/syllables.size(); //Rough estimate of distance between syllables
	float currentBoundry = 0;
	int boundryAmountInt = (int)boundryAmount;
	int currentBoundryInt = 0;

	for (int i=0, syllableSize=syllables.size(); i<syllableSize;++i)
	//for every syllable in the word
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
    for (int i=0, SWCSize=syllableWrongCount.size(); i<SWCSize; ++i)
    {
        SSG::MSL->addToTotal(syllables[i],syllableWrongCount[i]);
    }
}

//public
word::word(bool safteyMechanism)
//Requiring a bool to call a non functioning constructor removes the ability for it to be auto called.
{
	cerr << "WARNING DEFAULT CONSTRUCTER BEING CALLED, OBJECT *MUST* ALREADY BE CONSTRUCTED!" << endl;
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
	vector<string> wordVec(split(wordline)); //Delimit the wordline based off + char
	wordC = wordVec[0]; //set wordCapitalised to the word

	//TODO? insert code to add flags
	//TODO? Update i start value when flags added
	for (unsigned int i=1, wordVecSize=wordVec.size(); i < wordVecSize; ++i)
	//Start from 1 so the wordC is not added as a syllable
	{
		if (wordVec[i] == "#DEF") //If a the #DEF flag, next item must be the
		{
			definition = wordVec[i+1]; //if current string is #DEF the next string must be the definition
			break; //Needed to stop definition being added as a syllable
		}
		//Can now assume not a flag or a definition
		syllables.push_back(wordVec[i]);
	}

	SSG::MSL->addSyllables(syllables); //Add the syllables to the MSL, they will only be added if not added before.
	determineSyllables(); //Find the syllable boundries

	wordFlags = {false,false,false,false,false,false,false,false}; //Set all the flags to false, if made use of in future the flags will be set by a wordVec item.

}

word::~word()
{
	;//All destruction occurs when destorying member objects, virtual destructor needed for correct destruction of badWord when stored as a word*
}

bool word::wordCorrect()
{
	weight -= 0.1; //Reduce the weight
	if (weight<0) //Limit the min weight to 0
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
