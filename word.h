#ifndef word_h
#define word_h

#include "SSG.h"
#include <string>

void printVector(const vector<string>& sV); //Early declaration
void printVector(const vector<int>& sV);

class word
{
private:
	string generateBadWordLine(vector<int>& syllableWrongCount)
	{
		string badWordLine = wordC + "+1.0";
		for (int i =0; i<syllableWrongCount.size(); i++)
		{
			badWordLine += ("+" + to_string(syllableWrongCount[i]));
		}
		return badWordLine;

	}
protected:
	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void split(const string &s,const char delim, vector<string>& elems) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    string item; //string to hold the newly found item
	    while (getline(ss, item, delim)) { //while still data left in the stringstream read until you encounter the delimiter, store the stream in the item string, execute loop, repeat
	    	if (!item.empty())
	        	elems.push_back(item);
	    }
	}


	vector<string> split(const string &s, const char delim='+') {
	    vector<string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	int determineSyllables()
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

	vector<int> determineWrongSyllables(const string& attempt)
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


public:
	string wordC;//wordCapitalised
	vector<string> syllables;
	vector<int> syllablePositions;
	string definition;
	vector<bool> wordFlags;
	//int size = 0;

	word(bool safteyMechanism)
	{
		cout << "WARNING DEFAULT CONSTRUCTER BEING CALLED, OBJECT *MUST* ALREADY BE CONSTRUCTED!" << endl;
	}

	word(const string& wordline) {
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

	}

	virtual ~word()
	{
		;//All destruction occurs when destorying member objects, virtual destructor needed for correct destruction of badWord
	}

	virtual void wordWrong(const string& attempt)
	{
		
		//improve by adding for loop to set all the values of syllableWrongCount to 0, then only update the incorrect syllables.
		/*
		cout << "Original wordWrong called" << endl;
		vector<int> syllableWrongCount;
		if (attempt.size() == wordC.size())
		{
			for (int i=0; i<wordC.size(); i++) //For every letter in the word
			{
				cout << "i:" << i << endl;
				if (wordC[i] != attempt[i]) //If the word letter is not the same as the attempt letter
				{
					for (int j=0; j<syllablePositions.size(); j++)
					{
						if (syllablePositions[j] >= i)
						{
							syllableWrongCount.push_back(1);
							if (j!=(syllablePositions.size()-1))
							{
								i = syllablePositions[j+1]-1; //start looking for if next syllable corrent
								break;
							}
							else
							{
								i=wordC.size();
							}
						}
					}
					cout << "Continueing when i=" << i << endl;
					continue;
				}
				if (i+1 == syllablePositions[syllableWrongCount.size()+1])
					syllableWrongCount.push_back(0);
				if (i==wordC.size()-1)
					syllableWrongCount.push_back(0);
			}
		}
		else
		{
			int endpoint;
			if (attempt.size()>wordC.size())
			{
				endpoint = wordC.size();
			}
			else
			{
				endpoint = attempt.size();
			}

			for (int i=0; i<endpoint; i++)
			{
				if (wordC[i] != attempt[i])
				{
					while (syllableWrongCount.size() < syllables.size());
					{
						syllableWrongCount.push_back(1);
					}
					break;
				}
				syllableWrongCount.push_back(0);
			}
		}
		*/

		cout << "Original wrongWord called!" << endl;
		vector<int> syllableWrongCount = determineWrongSyllables(attempt); //Create a vector to store the values for the badWord syllableWrongCount

		
		printVector(syllables);
		printVector(syllableWrongCount);
		printVector(syllablePositions);

		string badWordLine = generateBadWordLine(syllableWrongCount);

		cout << badWordLine << endl;
		

	}
	
};

#endif