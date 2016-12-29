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
	void badWord::constructorHelper(const string& badwordline)
	{
		//Assumes the word constructor has already been called
		vector<string> wordVector = split(badwordline);
		printVector(wordVector);

		if (wordVector[0] != wordC) //If trying to construct a badWord from the wrong word throw an error.
		{
			throw "WordSyncError";
		}

		//finish the badWord constructor
		cout << "Trying to convert " << wordVector[1] << " to float" << endl;
		weight = stof(wordVector[1]);

		for (unsigned int i=2; i<wordVector.size(); i++)
		{
			cout << "Trying to convert " << wordVector[i] << " to int" << endl;
			syllableWrongCount.push_back(stoi(wordVector[i]));
		}

		if (syllableWrongCount.size() != syllables.size())
		{
			cout << "Word mismatch found!" << endl;
			throw "WordSyllableCount";
		}

		wordFlags[0] = true;
		addToMSLTotal(syllableWrongCount);
		determineScore();
		//badWordCount++;
	}

//public

	badWord::badWord(const string& wordline, const string& badwordline) : word(wordline) //Call the word constructor to initialise default values
	{
		constructorHelper(badwordline);
	}

	badWord::badWord(const badWord& createFrom) : word(true)
	{
		//word(true);
		syllableWrongCount=createFrom.syllableWrongCount;
		weight=createFrom.weight ;
		wordC=createFrom.wordC;
		syllables=createFrom.syllables;
		definition=createFrom.definition;
		wordFlags=createFrom.wordFlags;
		syllablePositions=createFrom.syllablePositions;
	}

	badWord::badWord(const word* createFrom, string badWordLine) : word(true)
	{
		wordC=createFrom->getWord();
		syllables=createFrom->getSyllables();
		definition=createFrom->getDefinition();
		wordFlags=createFrom->getWordFlags();
		syllablePositions=createFrom->getSyllablePositions();
		wScore=createFrom->getwScore();
		constructorHelper(badWordLine);
	}

	string badWord::wordWrong(const string& attempt)
	{
		cout << "badWord wordWrong Called" << endl;

		vector<int> syllablesToIncrement = word::determineWrongSyllables(attempt);

		addToMSLTotal(syllablesToIncrement);

		weight += 0.2;

		return "badWord";
	}

	void badWord::determineScore()
	{
		//cout << "badWord determineScore called" << endl;
		wScoreHelper();
		wScore *= weight;
		//cout << "Final score for " << wordC << wScore << endl;
	}

	vector<int> badWord::getSyllableWrongCount() const {return syllableWrongCount;}

	string badWord::getBadWordLine()
	{
		string badwordLine = wordC + '+' + to_string(weight);

		for (int i=0; i<syllableWrongCount.size(); i++)
		{
			badwordLine += '+' + to_string(syllableWrongCount[i]);
		}

		return badwordLine;
	}
