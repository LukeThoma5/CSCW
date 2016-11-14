#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

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

void createRandomWordWrongCounts() //Fill the MSL with random wrongCounts to test sorting
{
	const int HIGH = 450;
	const int LOW = 0;

	for (unsigned int i=0; i<SSG::MSL.wrongCount.size(); i++)
	{
		SSG::MSL.wrongCount[i] = randNG(LOW,HIGH); //Valgrind does not like this call
		//SSG::MSL.wrongCount[i] = 0; //This can be used to memory leak testing
	}
}


int letterToInt(char c)
{
	return static_cast<int>(c)-65;
}

void callTest(word* myWord)
{
	myWord->wordWrong("Hello");
}

word& returnRef(word* myWord)
{
	return *myWord;
}

void testme(bool why)
{
	cout << why << endl;
}

int main(int argc, char const *argv[])
{

	/*
	SSG::MSL = masterSyllablesList();
	SSG::goodWordList = NULL;
	SSG::badWordList = NULL;
	*/

	/*
	vector<string> dictVec = loadDictFile();

	//cout << dictVec[0] << endl;
	//cout << dictVec[dictVec.size()-1] << endl;

	vector<word> wordList;

	//Load all the words from the word list
	for (unsigned int i=0; i<dictVec.size(); i++)
	{
		//word* newWord = new word(dictVec[i]);
		//wordList.push_back(*newWord);
		//delete newWord;
		wordList.push_back(word(dictVec[i]));
	}

	/*
	for (unsigned int i=0; i<3; i++)
	{
		cout << wordList[i].wordC  << ": "<< wordList[i].definition << endl;
	}
	*/

	wordContainer allWords("finalDictwithDef.txt");


	SSG::goodWordList = &allWords;
	//SSG::badWordList = &allBadWords;

	cout << SSG::goodWordList->wordList[1]->wordC << endl;

	printVector(SSG::MSL.syllables);

	// printVector(SSG::MSL.wrongCount);

	createRandomWordWrongCounts();

	printVector(SSG::MSL.wrongCount);

	SSG::MSL.sortList();

	cout << "List sorted ---------------------------------------------" << endl;

	printVector(SSG::MSL.syllables);

	printVector(SSG::MSL.wrongCount);

	//cout << findWordLocation("ALLURING",wordList) << endl;



	cout << allWords.findWordLocation("BOIL") << endl;
	cout << SSG::goodWordList->findWordLocation("BOIL") << endl;
	int boilLocation = allWords.findWordLocation("ADEQUATELY");

	cout << boilLocation << endl;

	cout << allWords.wordList[boilLocation]->wordC << endl;

	//allWords.wordList[boilLocation]->wordWrong("ADEKUAT3IYY",SSG::badWordList);

	allWords.wordWrong(boilLocation,"ADEKUAT3IYY",&allWords);

	//badWord* test2 = SSG::badWordList->getBadWord(0);

	//printVector(SSG::badWordList->getBadWord(0)->syllableWrongCount);

	//printVector(static_cast<badWord*>(SSG::badWordList->wordList[0])->syllableWrongCount);

	wordCC SpellingWords("finalDictwithDef.txt", "wrongWords.txt");

	SpellingWords.generatewScore();


	//callTest(allWords.wordList[0]);




	return 0;
}
