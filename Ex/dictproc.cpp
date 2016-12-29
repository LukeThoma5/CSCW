#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "randng.h" //My random number generator header
#include "masterSyllableList.h" //MSL declaration
#include "word.h"
#include "SSG.h"
#include "badWord.h"
#include "wordContainer.h"

using namespace std;

void printVector(const vector<string>& sV)
{
	for (unsigned int i=0; i < sV.size(); i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

void printVector(const vector<int>& sV)
{
	for (unsigned int i=0; i < sV.size(); i++)
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
		//SSG::MSL.wrongCount[i] = randNG(LOW,HIGH); //Valgrind does not like this call
		SSG::MSL.wrongCount[i] = 0; //This can be used to memory leak testing
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

int main(int argc, char const *argv[])
{
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

	wordContainer allBadWords(allWords,"wrongWords.txt");


	SSG::goodWordList = &allWords;
	SSG::badWordList = &allBadWords;

	cout << SSG::goodWordList->wordList[1]->wordC << endl;

	cout << allBadWords.wordList[0]->wordC << endl;

	printVector(SSG::MSL.syllables);

	//printVector(SSG::MSL.wrongCount);

	createRandomWordWrongCounts();

	printVector(SSG::MSL.wrongCount);

	SSG::MSL.sortList();

	cout << "List sorted ---------------------------------------------" << endl;

	printVector(SSG::MSL.syllables);

	printVector(SSG::MSL.wrongCount);

	//cout << findWordLocation("ALLURING",wordList) << endl;



	cout << allWords.findWordLocation("BOIL") << endl;
	int boilLocation = allWords.findWordLocation("ADEQUATELY");

	cout << boilLocation << endl;
	

	cout << allWords.wordList[boilLocation]->wordC << endl;



	allWords.wordList[boilLocation]->wordWrong("ADEKUAT3IYY");

	//callTest(allWords.wordList[0]);

	cout << allWords.wordList[1000]->wordC << endl;

	allWords.printWordIndexBoundaries();
	allWords.deleteWord(1000);
	cout << "-----------------------------------" << endl;
	allWords.printWordIndexBoundaries();

	

	return 0;
}
