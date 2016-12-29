#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "SSG.h"
#include "word.h"

#include "math.h"
#include <complex>

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
		wScore = 0.0; //Reset wScore to start from a clean slate
		for (unsigned int i=0; i<syllables.size(); i++) //For every syllable this word has
		{
			/* //Old serial search through MSL to find syllable
			for (unsigned int j=0; j<SSG::MSL.size(); j++) //For every syllable in the masterSyllableList
			{
				if (syllables[i] == SSG::MSL[j]) //If the syllables match
				{
					//cout << syllables[i] << " found at " << j << endl;
					wScore += SSG::MSL.getSyllableWCount(j); //Add the wrongCount of the masterSyllableList to the total wScore
					if (SSG::MSL.getSyllableWCount(j) != SSG::MSL.getSyllableWCount(syllables[i]))
						cout << "MAJOR ERROR IN wSCOREHELPER!!! FIX IMEDIATELY" << endl;
					break; //Stop searching through the MSL and move on to the next syllable of this word
				}
			}
			*/
			wScore += SSG::MSL->getSyllableWCount(syllables[i]); //Gets the wrongCount for the string passed, using an unordered_map
		}

		const int offset = 15; //Use this to alter the affect that wordSize has on end wScore, a higher value makes word length matter less
		const int ideal = 7; //The target word size
		const int denom = ideal + offset; //A compile time constant to reduce the amount of math required at runtime
		signed int wordSize = wordC.size(); //Forcing the size of the word from an unsigned int to a signed int so it wont underflow

		#ifdef WORDDEBUG
		//Error checking and Algorithm testing code
		if ((denom-abs(wordSize-ideal))/(denom) > 1.0)
		{
			cout << "wScore for " << wordC << " is " << wScore << endl;
			cout << "The penalty is " << (abs(wordSize-ideal)+offset)/(denom) << endl;
			cout << "The absolute value is " << abs(wordSize-ideal) << " the value was " << wordSize-ideal << endl;
			cout << "The final score is: " << (wScore/syllables.size()) * (denom-abs(wordSize-ideal))/(denom) << endl;
		}

		double numerator = (denom-abs(wordSize-ideal));
		double penalty = numerator/denom;

		float rwScore = (wScore*penalty)/syllables.size();
		float twScore = (wScore*(denom-abs(wordSize-ideal)))/(denom*syllables.size());

		if (rwScore != twScore)
		{
			cout << "An error has occured " << rwScore << " != " << twScore << endl;
			cout << rwScore << " " << twScore << " " << rwScore-twScore << endl;
		}

		float wTotal = wScore;
		wScore = rwScore;

		if (wScore == 0.0)
		{
			float numerator = (denom-abs(wordSize-ideal));
			float penalty = numerator/denom;
			cout << "ZERO wScore!!!" << endl;
			cout << "wScore for " << wordC << " is " << wTotal << endl;
			cout << "The penalty is " << ((denom-abs(wordSize-ideal))/(denom)) << " which should be " << penalty << endl;
			cout << "The numerator is " << numerator << endl;
			cout << "The denomenator is " << denom << endl;
			cout << "The absolute value is " << abs(wordSize-ideal) << " the value was " << wordSize-ideal << endl;
			cout << "The final score is: " << ((wScore/syllables.size()) * ((denom-abs(wordSize-ideal))/(denom))) << endl;
		}
		#endif

		/*
		Documentation
		Average syllable contribution = [Sum of syllable Wrong Count for this word] / [# Number of syllables for this word]
		is multiplied by a penalty <= 1.0 which is there to reduce the effect of very long words.
		penalty = [offset+ideal-magnitude(length-ideal)] / [offset+ideal]
		magnitude(length-ideal) gives you the number of letters away from the ideal number.
		denom = k = offset+ideal

		wScore = [Average syllable contribution] * penalty

		In its 'simplest' form it is:

		numerator = [[Sum of syllable Wrong Count for this word] * (K-magnitude(length-ideal))]
		denomenator = [K(# Number of syllables for this word)]

		wScore = numerator / denomenator

		To decrease the effect that being near the ideal size has, increase the offset.
		*/
		wScore = (wScore*(denom-abs(wordSize-ideal)))/(denom*syllables.size());


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

	void word::addToMSLTotal(const vector<int>& syllableWrongCount)
	{
		for (int i=0; i<syllableWrongCount.size(); i++)
		{
			//cout << syllables[i] << "wrong " << syllableWrongCount[i] << endl;
			SSG::MSL->addToTotal(syllables[i],syllableWrongCount[i]);
		}
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
		wScore *= weight; //Makes good words less valuable than badWords
		//cout << "Final score for " << wordC << wScore << endl;
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

		SSG::MSL->addSyllables(syllables); //Add the syllables to the MSL, they will only be added if not added before.
		determineSyllables();

		for (unsigned int i=0; i<8; i++) //Set all flags to 0
		{
			wordFlags.push_back(false);
		}

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

		//addToMSLTotal(syllableWrongCount);

		return badWordLine;

		//badWord mybadWord("ADEQUATELY+AE1+D+AH0+K+W+IH0+T+L+IY0+#DEF+In an adequate manner.","ADEQUATELY+1.0+0+0+0+0+0+0+0+0+0");

		//badWord* badWordToAdd = new badWord(this,badWordLine);

		//badWordContainer->addBadWord(this,badWordLine);

	}
