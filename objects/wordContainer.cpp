#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <math.h>

#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordContainer.h"

using namespace std;

//Test mergesort and wordPos

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);

//protected

	vector<string> wordContainer::loadDictFile(string filename)
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

	vector<int> wordContainer::splitVector(const vector<int>& inVector, int mode)
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

	vector<int> wordContainer::containerMergeSort(const vector<int>& inVector)
	{
		vector<int> leftVector = splitVector(inVector,0);  //Create left hand side with mode 0
		vector<int> rightVector = splitVector(inVector,1); //Create right hand side with mode 1

		int leftVectorSize = leftVector.size(); //Trying to minimise the amount of function calls
		int rightVectorSize = rightVector.size();

		if (leftVectorSize != 1) //if left needs to be sorted
			leftVector = containerMergeSort(leftVector); //sort the left hand side
		if (rightVectorSize != 1)
			rightVector = containerMergeSort(rightVector);

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
					if (wordList[leftVector[leftPos]]->getwScore() >= wordList[rightVector[rightPos]]->getwScore()) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
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

	int wordContainer::findWordInsertionPoint(const string& searchWord)
	{
		for (int i=0; i<wordList.size(); i++)
		{
			int compResult = stringCompare(searchWord,wordList[i]->getWord());
			cout << "Current word location " << wordList[i]->getWord() << " " << i  << " with compResult" << compResult << endl;
			if (compResult < 0)
			{
				cout << "Current word location " << wordList[i]->getWord() << " " << i  << endl;
				return i;
			}

		}
		return wordList.size(); //If it doesn't find a location, it must be the highest word so add to back.
	}

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void wordContainer::findWord(const string &s, string& badwordWord) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    getline(ss, badwordWord, '+');
	    	if (badwordWord.empty())
	        	cout << "Waring attempting to create badword with an empty word string!" << endl;
	}

	void wordContainer::refreshwScores()
	{
		for (unsigned int i=0; i<wordList.size(); i++)
		{
			wordList[i]->determineScore();
		}
	}

	void wordContainer::fillWordPos()
	{
		wordPos.clear(); //Removes previous positions
		wordPos.reserve(wordList.size()); //Reduces the need to move the vector in memory by saying out the gate the miniumum space needed
		for (unsigned int i=0; i<wordList.size(); i++)
		{
			wordPos.push_back(i);
		}
	}

//public


	void wordContainer::removeWord(int wordPosition)
	{
		wordList.erase(wordList.begin()+wordPosition); //Remove the word* at position wordPosition. erase function takes a vector iterator which points at the word to be removed, begin function returns an iterator pointing to the start which is then incremented by wordPosition

		//Update wordPos to reflect the fact that a word has been removed
		for (int i=0; i<wordPos.size(); i++) //For every item in the list
		{
			if (wordPos[i] == wordPosition) //If it was the word that was erased, erase its position
				wordPos.erase(wordPos.begin()+i);
			if (wordPos[i] > wordPosition) //If the word was later in the list than the erased word, reduce the words index by 1
				wordPos[i]--;
		}
	}

	void wordContainer::deleteWord(int wordPosition)
	{
		cout << "DELETING " << wordList[wordPosition]->getWord() << endl;
		delete wordList[wordPosition];
		removeWord(wordPosition);
	}

	int wordContainer::getABSIndex(const int& wordToGet)
	{
		return wordPos[wordToGet];
	}

	int wordContainer::addWord(word* wordToAdd)
	{
		//cout << "Trying to add word " << wordToAdd->getWord() << endl;
		//Needs to be improved/fixed
		//ADD the wordPos fixing!!
		int insertPos = findWordInsertionPoint(wordToAdd->getWord()); //Find the alphabetical location in the list to add the word to
		//cout << "Insertion position " << insertPos << endl;

		//wordList.push_back(wordToAdd);
		//Fix abstraction
		for (int i=0; i<wordPos.size(); i++) //for every word in the abstraction
		{
			if (wordPos[i] >= insertPos) //if the index for wordList held in the abstraction layer is greater than the insertionPoint the index in the abstraction layer needs to be incremented needs to be shifted to the right one.
				wordPos[i]++;
		}
		wordList.insert(wordList.begin()+insertPos,wordToAdd); //Add the word at the insertionPoint
		wordPos.push_back(insertPos); //Add the insertionPoint to the back of abstraction layer so everything else stays in order. Regardless of the words score it will be only acessible at the end until refreshwScores called.

		/* //Test code for printing the words around the new inserted word for manual inspection.
		for (int i=insertPos-1;i<insertPos+2;i++)
		{
			cout << i << ": " << wordList[i]->getWord() << endl;
		} */
		cout << "addWord Complete" << endl;

		int wordPosSize = wordPos.size(); //Convert to signed int so -1 doesn't cause an underflow when size is 0
		//return insertPos;
		return wordPosSize-1;
	}

	word* wordContainer::at(int wordToGet)
	{
		//This function gets the word from the wordPos list, eg sorted by wScore
		return wordList[wordPos[wordToGet]];
	}

	int wordContainer::findWordLocation(const string& wordToFind)
	{
		return binSearch(wordToFind,0,wordList.size());
	}

	wordContainer::wordContainer(string filename)
	{
		vector<string> dictionaryVector = loadDictFile(filename); //get file in vector format
		for (unsigned int i=0; i<dictionaryVector.size(); i++) //for every line of the dictionary
		{
			word* wordToAdd = new word(dictionaryVector[i]);
			wordList.push_back(wordToAdd); //create a word and store it in the wordList as a pointer
		}
		//All words now generated
	}


	wordContainer::wordContainer()
	{
		cout << "WARNING DEFAULT WORDCONTAINER CONSTRUCTOR IS BEING CALLED" << endl;
	}

	/* //Functionality moved to wordCC
	void wordContainer::wordWrong(int wordPosition, std::string attempt, wordContainer* containerToAddTo)
	{
		cout << wordList[wordPosition]->getWord() << endl;
		string badWordLine = wordList[wordPosition]->wordWrong(attempt);
		cout << badWordLine << endl;
		badWord* badWordToAdd = new badWord(wordList[wordPosition],badWordLine);
		cout << badWordToAdd->getWordFlags()[0] << endl;
		delete badWordToAdd; //Remove when adding to a container
		if (containerToAddTo == this)
			cout << "You are trying to add to your self!" << endl;
		//Add overloaded functions that don't include which removing words or deleting words ect. Improve the add words, make indexing turn off able.
	}
	*/

	wordContainer::~wordContainer()
	{
		cout << "wordContainer object being destroyed, all its words will be deleted!" << endl;
		while (!wordList.empty())
		{
			delete wordList.back(); //returns the pointer at the back of the wordList and deletes the object it points to. As the destructor call is virtual the correct call will be made so no need to cast the pointer to the derived type
			wordList.pop_back(); //removes the last pointer from wordList which is destroyed
		}
	}

	void wordContainer::sortWordContainer()
	{
		cout << "Sorting wordContainer with " << wordList.size() << " words" << endl;
		refreshwScores();
		fillWordPos();
		wordPos = containerMergeSort(wordPos);
		int stop = wordPos.size();
		if (stop > 10)
			stop = 10;
		printVector(wordPos,0,stop);
		for (unsigned int i=0; i<stop; i++)
		{
			cout << "Word: " << wordList[wordPos[i]]->getWord() << " at " << wordPos[i] << " with a value of " << wordList[wordPos[i]]->getwScore() << endl;
		}
	}

	void wordContainer::printWordContainer()
	{
		for (int i=0; i<wordList.size(); i++)
			cout << i << ": " << wordList[i]->getWord() << " ";
		cout << endl;

		for (int i=0; i<wordPos.size(); i++)
		{
			cout << i << ": " << wordPos[i] << " ";
		}
		cout << endl;
	}

	void wordContainer::DisplaywScores(int start, int stop)
	{
		if (stop ==-1)
			stop = wordList.size();
		for (unsigned int i=start; i<stop; i++)
		{
			cout << wordList[i]->getwScore() << ",";
		}
		cout << endl;
	}

	int wordContainer::size() const
	{
		return wordList.size();
	}

	int wordContainer::stringCompare(const string& str1, const string& str2)
	{

		if (str1 == str2)
			return 0;

		int str1size = str1.size(); //Force the string size to a signed int
		int str2size = str2.size();
		int stop = str1size;

		int retInt = -1;

		if (str1size != str2size)
		{
			if (str1size > str2size)
			{
				stop = str2size;
				retInt = 1;
			}
		}

		int s1pos = 0;
		int s2pos = 0;
		while (s1pos <  str1.size() && s2pos < str2.size())
		{
			char chr1 = str1.at(s1pos);
			char chr2 = str2.at(s2pos);
			//cout << "Checking char " << chr1 << " " << chr2 << endl;
			if (chr1 == chr2)
			{
				s1pos++;
				s2pos++;
				continue;
			}

			int chr1i = static_cast<int>(chr1);

			if (chr1i < 65)
			{
				s1pos++;
				continue;
			}

			int chr2i = static_cast<int>(chr2);

			if (chr2i < 65)
			{
				s2pos++;
				continue;
			}

			if (chr1 > chr2)
				return 1;
			return -1;
		}

		//cout << "COULD NOT DECIDE" << s1pos << " " << s2pos << endl;
		return retInt;

	}

	int wordContainer::binSearch(const std::string& comp, int start, int stop)
	{

		//string pause;
		//cin >> pause;
		//cout << "----------------------------------------------------------------------\n";
		//cout << "Start point " << start << " stop point " << stop << endl;
		float midFloat =start+stop;
		midFloat = midFloat / 2;
		int midpoint = ceil(midFloat);
		//cout << "Midpoint is " << midpoint << " " << midFloat << endl;
		//cout << "Checking " << wordList[midpoint]->getWord() << endl;
		if (start == stop) //Bottom out recursion, if can't split again
		{
			if (comp == wordList[start]->getWord())
				return start;
			return -1;
		}

		if (stop-start == 1)
		{
			//int compres = stringCompare(comp,wordList[start]->getWord();
			//cout << "Checking " << wordList[start]->getWord() << endl;
			if (comp ==  wordList[start]->getWord())
				return start;
			//cout << "Checking " << wordList[stop]->getWord() << endl;
			if (comp == wordList[stop]->getWord())
				return stop;
			return -1;
		}

		int comparisonResult = stringCompare(comp,wordList[midpoint]->getWord()); //comp.compare(wordList[midpoint]->getWord());

		//cout << "Expecting " << comp.compare(wordList[midpoint]->getWord()) << " got " << comparisonResult << endl;

		if (comparisonResult == 0)
			return midpoint;
		if (comparisonResult > 0)
		{
			//if (stop-midpoint == 1)
				//return binSearch(comp,midpoint,midpoint);
			//cout << "searching at " << midpoint << " " << stop << endl;
			return binSearch(comp,midpoint,stop);
		}
		if (comparisonResult < 0)
		{
			//if (midpoint-start == 1)
				//return binSearch(comp,start,start);
			//cout << "searching at " << start << " " << midpoint << endl;
			return binSearch(comp,start,midpoint);
		}


		cout << "An error has occured in searching for word " << comp << endl;
	}

	bool wordContainer::wordCorrect(const int& correctWord)
	{
		if (correctWord > size() || correctWord < 0)
			throw "wordContainer out of range";
		word* cWord = at(correctWord);
		cout << cWord->getWord() << " is correct with a weight of " << cWord->getWeight() << " down to ";
		cWord->wordCorrect();
		float wordWeight = cWord->getWeight();
		cout << wordWeight << endl;

		if (wordWeight == 0.0)
			return true;
		return false;
	}
