#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include "randng.h" //My random number generator header
#include "masterSyllableList.h" //MSL declaration
#include "word.h"
#include "badWord.h"
#include "SSG.h"
#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"

using namespace std;

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	masterSyllablesList MSL; //MasterSyllablesList
	wordContainer* goodWordList = NULL; //Allows the currently worked on wordList to be updated and interchanged
	wordContainer* badWordList = NULL; //Defaulting to null prevents manipulating objects that don't exist.
}



void printVector(const vector<string>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=0; i < sV.size(); i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void printVector(const vector<int>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=start; i < stop; i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void printVector(const vector<bool>& sV, int start=0, int stop=-1)
{
	if (stop==-1)
		stop=sV.size();
	for (unsigned int i=start; i < stop; i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

bool testWordContainerSearch(wordContainer& goodWords)
{
    //string.compare has issues when passed antic and anti-Federalist
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

int main(int argc, char const *argv[]) {
    wordContainer goodWords("finalDictwithDef.txt");
    //wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt
    testWordContainerSearch(goodWords);
	cout << "Tests complete" << endl;
    return 0;
}
