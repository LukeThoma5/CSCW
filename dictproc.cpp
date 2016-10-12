#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "randng.h" //My random number generator header

using namespace std;

int badWordCount = 0;

void printVector(const vector<string>& sV); //Early declaration

class masterSyllablesList
{
private:
	vector<int> syllablePos; //For mergesort

	void printVector(const vector<int>& sV) //Remove
	{
		for (unsigned int i=0; i < sV.size(); i++)
		{
			cout << i << ":" << sV[i] << ' ';
		}
		cout << endl;
	}

	vector<int> splitVector(const vector<int> inVector, int mode)
	{
		int start;
		int end;
		if (!mode) //if 0
		{
			start = 0;
			end = inVector.size()/2;
		}
		else
		{
			//if not 0
			start = inVector.size()/2;
			end = inVector.size();
		}
		vector<int> returnVector;
		for (int i=start; i<end; i++)
			returnVector.push_back(inVector[i]); //fill the vector with the values from original
		return returnVector;
	}

	vector<int> syllableMergeSort(const vector<int>& inVector)
	{
		vector<int> leftVector = splitVector(inVector,0);  //Create left hand side with mode 0
		vector<int> rightVector = splitVector(inVector,1); //Create right hand side with mode 1

		int leftVectorSize = leftVector.size(); //Trying to minimise the amount of function calls
		int rightVectorSize = rightVector.size();

		if (leftVectorSize != 1) //if left needs to be sorted
			leftVector = syllableMergeSort(leftVector); //sort the left hand side
		if (rightVectorSize != 1)
			rightVector = syllableMergeSort(rightVector);

		//Now assumed the left and right vectors are sorted
		int leftPos = 0; //Simple vector position pointers
		int rightPos = 0;

		vector<int> returnVector;

		while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
		{
			if (leftPos == leftVectorSize) //If out of left hand values add a right
			{
				returnVector.push_back(rightVector[rightPos++]); //add the next right value to the return list and increment rightPos counter
			}
			else 
			{
				if (rightPos == rightVectorSize) //If out of right hand values add a left
				{
					returnVector.push_back(leftVector[leftPos++]);
				}

				else
				{
					if (wrongCount[leftVector[leftPos]] >= wrongCount[rightVector[rightPos]]) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
					{
						returnVector.push_back(leftVector[leftPos++]); //Add the leftvector position
					}
					else
					{
						returnVector.push_back(rightVector[rightPos++]);
					}
				}
			}
		}

		return returnVector;

	}

	void enactMergeSort(const vector<int>& inVector)
	{
		syllablePos = syllableMergeSort(inVector);

		//printVector(syllablePos);
		//Set new positions

		/*
		cout << "Sorted list?" << endl;
		for (int j=0; j<syllablePos.size(); j++)
		{
			cout << syllablePos[j] << ": " << wrongCount[syllablePos[j]] << endl;
		}
		*/

		vector<string> syllablesTemp;
		vector<int> wrongCountTemp;
		vector<float> weightTemp;

		for (int i=0; i<syllablePos.size(); i++)
		{
			syllablesTemp.push_back(syllables[syllablePos[i]]);
			wrongCountTemp.push_back(wrongCount[syllablePos[i]]);
			weightTemp.push_back(weight[syllablePos[i]]);
		}

		syllables = syllablesTemp;
		wrongCount = wrongCountTemp;
		weight = weightTemp;
	}

public:
	vector<string> syllables;
	vector<int> wrongCount;
	vector<float> weight;

	vector<string> findUniqueSyllables(const vector<string>& inSyllables)
	{
		if (syllables.size() == 0)
		{
			return inSyllables;
		}
		vector<string> returnVector;
		bool unique = true;
		for (unsigned int i=0; i < inSyllables.size(); i++)
		{
			unique = true;
			for (unsigned int j=0; j<syllables.size(); j++)
			{
				if (inSyllables[i] == syllables[j])
				{
					unique = false;
					break;
				}
			}
			if (unique)
			{
				returnVector.push_back(inSyllables[i]);
			}
		}
		return returnVector;
	}

	void addSyllables(const vector<string>& inSyllables)
	{
		vector<string> syllablesToAdd = findUniqueSyllables(inSyllables);
		for (unsigned int i=0; i < syllablesToAdd.size(); i++)
		{
			syllables.push_back(syllablesToAdd[i]);
			wrongCount.push_back(0);
			weight.push_back(0.0);
		}
	}

	void sortList()
	{
		cout << "Sorting list" << endl;
		if (syllablePos.size() == 0)
		{
			for (int i=0; i<syllables.size(); i++)
			{
				syllablePos.push_back(i);
			}
		}
		else
		{
			for (int i=0; i<syllables.size(); i++)
			{
				syllablePos[i] = i;
			}
		}

		enactMergeSort(syllablePos);
	}
};

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	masterSyllablesList MSL; //MasterSyllablesList
}

class word
{
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


public:
	string wordC;//wordCapitalised
	vector<string> syllables;
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

	}

	virtual void wordWrong(const string& attempt)
	{
		cout << "Original wordWrong called" << endl;
	}
	
};

class badWord : public word { //New class badWord inherits from word, keep datamembers public

protected:
	void constructorHelper(const string& badwordline)
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

		badWordCount++;
	}

public:

	vector<int> syllableWrongCount;
	float weight;

	badWord(const string& wordline, const string& badwordline) : word(wordline) //Call the word constructor to initialise default values
	{
		constructorHelper(badwordline);
	}

	badWord(const badWord& createFrom) : word(true)
	{
		//word(true);
		syllableWrongCount=createFrom.syllableWrongCount;
		weight=createFrom.weight ;
		wordC=createFrom.wordC;
		syllables=createFrom.syllables;
		definition=createFrom.definition;
		wordFlags=createFrom.wordFlags;  
	}

	badWord(const word* createFrom, string badWordLine) : word(true)
	{
		wordC=createFrom->wordC;
		syllables=createFrom->syllables;
		definition=createFrom->definition;
		wordFlags=createFrom->wordFlags;
		constructorHelper(badWordLine);
	}

	virtual void wordWrong(const string& attempt)
	{
		cout << "badWord wordWrong Called" << endl;
	}
};

class wordContainer {

	vector<string> loadDictFile(string filename="finalDictwithDef.txt")
	{
		ifstream dictFile(filename); //in file stream object dictFile points to the dictionary file, dictionary file locatino given by filename parameter that is defaulted
		string line; //string to store the current line that has been read in
		vector<string> returnVec;
		while (getline(dictFile, line)) //getline takes an istream object and reads until a /n then stores the result in line. If no more lines to read returns false
		{
			returnVec.push_back(line); //Adds the new line to the vector
		}
		return returnVec;
	}

	int wordIndex[26];
	bool containsBadWords = false;

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void findWord(const string &s, string& badwordWord) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    getline(ss, badwordWord, '+');
	    	if (badwordWord.empty())
	        	cout << "Waring attempting to create badword with an empty word string!" << endl;
	}

public:
	vector<word*> wordList;

	void generateWordIndex()
	{
		char lastChar = 'Z'; //first character of the last searched word
		int wordListSize = wordList.size(); //minimise function calls
		for (int i=0; i<wordListSize; i++) //for every word in the word list
		{
			if (wordList[i]->wordC[0] != lastChar) //if the first character of the current word is difference to the last word
			{
				lastChar = wordList[i]->wordC[0]; //update the last character
				cout << lastChar << " found at " << i << endl; //testing purposes
				wordIndex[static_cast<int>(lastChar)-65] = i; //change the letter into a number 0-25 A-Z and store the first word of the new letters position in the index
			}
		}
	}

	void addWord(word* wordToAdd)
	{
		//Needs to be improved/fixed
		wordList.push_back(wordToAdd);
	}

	int findWordLocation(const string& wordToFind)
	{
		//unsigned int wordListSize = wordList.size(); //minimise unnessissary function calls
		int startPosition = wordIndex[static_cast<int>(wordToFind[0])-65]; //find the starting location to search for the word, wordToFind[0] is the first letter which is cast to an int between 65 and 90, removing 65 gives the letters position in the alphabet and so the position in the index array to get that letters starting position
		int stopPosition = wordIndex[static_cast<int>(wordToFind[0])-64];
		//cout << stopPosition-startPosition << endl;
		for (unsigned int i=startPosition; i< stopPosition; i++) //For every word with the same letter
		{
			if (wordToFind == wordList[i]->wordC) //if the word is found
				return i; //Return the position along the vector, also stops execution of loop
		}
		return -1; //If failed return an invalid position
	}

	wordContainer(string filename="finalDictwithDef.txt")
	{
		vector<string> dictionaryVector = loadDictFile(filename); //get file in vector format
		for (unsigned int i=0; i<dictionaryVector.size(); i++) //for every line of the dictionary
		{
			word* wordToAdd = new word(dictionaryVector[i]);
			wordList.push_back(wordToAdd); //create a word and store it in the wordList as a pointer
		}

		//All words now generated
		generateWordIndex(); //generate the index to speed up searches
		containsBadWords = false;
	}

	//Overloaded constructor, remove in future
	/*
	wordContainer(badWord& myBadWord)
	{
		containsBadWords = true;
		wordList.push_back(new badWord(myBadWord));
		generateWordIndex();
	}
	*/

	wordContainer(wordContainer& fullWordList, string filename="wrongWords.txt")
	{
		containsBadWords = true;
		vector<string> wrongWordVector = loadDictFile(filename);
		fullWordList.generateWordIndex();
		unsigned int wrongWordVectorSize = wrongWordVector.size(); 
		for (unsigned int i=0; i<wrongWordVectorSize; i++)
		{
			string badWordWord = "";
			findWord(wrongWordVector[i],badWordWord);
			word* originalWord = fullWordList.wordList[fullWordList.findWordLocation(badWordWord)];
			//wordList.push_back(new badWord());
			cout << originalWord->wordC << endl;

			wordList.push_back(new badWord(originalWord, wrongWordVector[i]));
		}

		generateWordIndex();
	}

	~wordContainer()
	{
		cout << "wordContainer object being destroyed, all its words will be deleted!" << endl;
		int i=0;
		if (!containsBadWords)
		{
			cout << "Standard destruction occured" << endl;
			while (!wordList.empty())
			{
				//cout << i++ << endl;
				delete wordList.back();
				wordList.pop_back();
			}
		}
		else
		{
			if (containsBadWords)
			{
				cout << "badWord destruction occured" << endl;
				while (!wordList.empty())
				{
					//cout << badWordCount << " bad words left in program" << endl;
					badWord* toDelete = static_cast<badWord*>(wordList.back());
					//cout << toDelete->wordC << endl;
					delete toDelete;
					//cout << --badWordCount << "bad words left in program2" << endl;
					wordList.pop_back();
				}
				
			}
		}
	}

};



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

	cout << allWords.wordList[allWords.findWordLocation("BOIL")]->wordC << endl;
	//callTest(allWords.wordList[0]);

	return 0;
}