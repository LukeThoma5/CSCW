#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "SSG.h"
#include "word.h"

#include "math.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);

//private
	string word::generateBadWordLine(vector<int>& syllableWrongCount)
	{
		string badWordLine = wordC + "+1.0";
		for (int i =0; i<syllableWrongCount.size(); i++)
		{
			badWordLine += ("+" + to_string(syllableWrongCount[i]));
		}
		return badWordLine;

	}

//protected
	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void word::split(const string &s,const char delim, vector<string>& elems) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    string item; //string to hold the newly found item
	    while (getline(ss, item, delim)) { //while still data left in the stringstream read until you encounter the delimiter, store the stream in the item string, execute loop, repeat
	    	if (!item.empty())
	        	elems.push_back(item);
	    }
	}


	vector<string> word::split(const string &s, const char delim) {
	    vector<string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	void word::wScoreHelper()
	{
		//cout << "Determining " <<  wordC << " score" << endl;
		wScore = 0.0;
		for (unsigned int i=0; i<syllables.size(); i++)
		{
			for (unsigned int j=0; j<SSG::MSL.size(); j++)
			{
				if (syllables[i] == SSG::MSL[j])
				{
					//cout << syllables[i] << " found at " << j << endl;
					wScore += SSG::MSL.getSyllableWCount(j);
				}
			}
		}

		//wScore = wScore/(sqrt(syllables.size())*0.2); //Reduce the effect that having a long word has on its score.

		//wScore = wScore* (200 - pow(syllables.size()-5,2));

		//Improve adjustment for wordSize

	}

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

	vector<int> word::determineWrongSyllables(const string& attempt)
	{
		vector<int> syllableWrongCount; //Create a vector to store the values for the badWord syllableWrongCount
		syllableWrongCount.reserve(syllables.size()); //To prevent unnessissary memory management make sure that the vector is atleast as long as the amount of syllables
		int attemptEnd = wordC.size();
		if (attempt.size() != wordC.size()) //If they are not the same length determine how far is safe to check so the program doesn't crash
		{
			if (attempt.size() < wordC.size())
				attemptEnd = attempt.size();
			//Reverse not nessissary as attemptEnd is initalised to wordC.size()
		}

		int syllableEndPos;
		bool isSyllableWrong = false;
		for (int i=0; i<syllables.size(); i++)
		{
			//cout << "Doing Checks for Syllable: " << i << endl;
			if (i != (syllables.size()-1))
				syllableEndPos = syllablePositions[i+1]; //The place to stop checking the current syllable is the position of the start of the next
			else
			{
				//cout << "syllables size: "  << syllables.size() << endl;
				//cout << "Attempt size: " << attempt.size() << endl;
				//cout << "WordC size: " << wordC.size() << endl;
				syllableEndPos = wordC.size(); //If on the last syllable, the place to stop checking at is the end of the word
				//cout << "syllableEndPos: " << syllableEndPos << endl;
			}
			isSyllableWrong = false; //Has an incorrect syllable been found in the current syllable
			for (int j=syllablePositions[i]; j<syllableEndPos; j++)
			{
				//cout << "Checking syllable: " << i << endl;
				if (syllableEndPos > attemptEnd) //If the syllable will overrun, the syllable must be wrong
				{
					isSyllableWrong = true;
					break;
				}
				if (attempt[j] != wordC[j]) //If the attempt has a wrong letter within the current syllable break
				{
					isSyllableWrong = true;
					break;
				}
			}
			if (isSyllableWrong) //If a wrong letter has been found mark the syllable as wrong
			{
				syllableWrongCount.push_back(1);
			}
			else
			{
				syllableWrongCount.push_back(0);
			}
		}

		if (attempt.size() > wordC.size())
		{
			syllableWrongCount[syllableWrongCount.size()-1] = 1;
		}

		return syllableWrongCount;
	}


//public

	//int size = 0;

	word::word(bool safteyMechanism)
	{
		cout << "WARNING DEFAULT CONSTRUCTER BEING CALLED, OBJECT *MUST* ALREADY BE CONSTRUCTED!" << endl;
	}

	void word::determineScore()
	{
		wScoreHelper();
		wScore *= 0.5; //Makes good words less valuable than badWords
		//cout << "Final score for " << wordC << wScore << endl;
	}

	word::word(const string& wordline) {
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

		SSG::MSL.addSyllables(syllables); //Add the syllables to the MSL, they will only be added if not added before.
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

	string word::wordWrong(const string& attempt)
	{

		cout << "Original wrongWord called!" << endl;
		vector<int> syllableWrongCount = determineWrongSyllables(attempt); //Create a vector to store the values for the badWord syllableWrongCount


		printVector(syllables);
		printVector(syllableWrongCount);
		printVector(syllablePositions);

		string badWordLine = generateBadWordLine(syllableWrongCount);

		cout << badWordLine << endl;

		return badWordLine;

		//badWord mybadWord("ADEQUATELY+AE1+D+AH0+K+W+IH0+T+L+IY0+#DEF+In an adequate manner.","ADEQUATELY+1.0+0+0+0+0+0+0+0+0+0");

		//badWord* badWordToAdd = new badWord(this,badWordLine);

		//badWordContainer->addBadWord(this,badWordLine);

	}
