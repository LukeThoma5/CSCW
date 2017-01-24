#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "../headers/word.h"
#include "../headers/badWord.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);

//protected
inline void badWord::constructorHelper(const string& badwordline)
//inline removes function call overhead but slightly increases compiled program size
//inlines are typesafe preprocessor marcros
{
	//Assumes the word constructor has already been called
	vector<string> wordVector = split(badwordline); //Split up the bad word line based of +
	//printVector(wordVector);

	if (wordVector[0] != wordC) //If trying to construct a badWord from the wrong word throw an error.
	{
		cerr<<"WordSyncError"<<endl;;throw "WordSyncError";
	}

	//cout << "Trying to convert " << wordVector[1] << " to float" << endl;
	weight = stof(wordVector[1]); //Set the weight to the weight in badwordline

	for (int i=2, wordVectorSize=wordVector.size(); i<wordVectorSize; ++i)
	//Start from 2 eg skip word and weight
	{
		//cout << "Trying to convert " << wordVector[i] << " to int" << endl;
		syllableWrongCount.push_back(stoi(wordVector[i])); //add to the wrong count the syllables wrong count
	}

	if (syllableWrongCount.size() != syllables.size()) //If the passed in word line didn't have the correct amount of wrongCounts
	{
		//Print to stderr and throw
		cerr << "Word mismatch found!" << endl;
		throw "WordSyllableCount";
	}

	wordFlags[0] = true; //Set word as badWord
	addToMSLTotal(syllableWrongCount); //Increase the MSL values by the wrongCounts
	determineScore(); //Determine wScore
}

//public

	badWord::badWord(const string& wordline, const string& badwordline) : word(wordline) //Call the word constructor to initialise default values
	{
		constructorHelper(badwordline);
	}

	badWord::badWord(const badWord& createFrom) : word(true) //Tell word all construction happening in badWord
	//badWord copy constructor
	{
		syllableWrongCount=createFrom.syllableWrongCount;
		weight=createFrom.weight ;
		wordC=createFrom.wordC;
		syllables=createFrom.syllables;
		definition=createFrom.definition;
		wordFlags=createFrom.wordFlags;
		syllablePositions=createFrom.syllablePositions;
	}

	badWord::badWord(const word* createFrom, string badWordLine) : word(true)
	//Create a word from a currently goodword and a badwordline
	{
		//Copy all the word values
		wordC=createFrom->getWord();
		syllables=createFrom->getSyllables();
		definition=createFrom->getDefinition();
		wordFlags=createFrom->getWordFlags();
		syllablePositions=createFrom->getSyllablePositions();
		wScore=createFrom->getwScore();
		//Make into a badword
		constructorHelper(badWordLine);
	}

	string badWord::wordWrong(const string& attempt)
	{
		cout << "badWord wordWrong Called" << endl; //Debugging message to help ensure correct polymorphism
		vector<int> syllablesToIncrement = word::determineWrongSyllables(attempt); //Find which syllables have been wrong
		addToMSLTotal(syllablesToIncrement); //Increment the MSL on the wrong syllables
		weight += 0.2; //Increase the weight which increases wScore on next calculation
		return "badWord"; //Allows checking of type
	}

	void badWord::determineScore()
	{
		//cout << "badWord determineScore called" << endl; //Debugging message to help ensure correct polymorphism
		wScoreHelper();
		wScore *= weight;
		//cout << "Final score for " << wordC << wScore << endl;
	}

	vector<int> badWord::getSyllableWrongCount() const {return syllableWrongCount;}

	string badWord::getBadWordLine()
	{
		string badwordLine = wordC + '+' + to_string(weight); //Add the word and weight
		//For every syllable
		for (int i=0, syllWCSize=syllableWrongCount.size(); i<syllWCSize; ++i)
			badwordLine += '+' + to_string(syllableWrongCount[i]); //Add its wrongCount delimited by a +
		return badwordLine;
	}
