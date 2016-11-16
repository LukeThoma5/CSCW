#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "word.h"
#include "badWord.h"
#include "wordContainer.h"

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
					if (wordList[leftVector[leftPos]]->wScore >= wordList[rightVector[rightPos]]->wScore) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
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

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void wordContainer::findWord(const string &s, string& badwordWord) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    getline(ss, badwordWord, '+');
	    	if (badwordWord.empty())
	        	cout << "Waring attempting to create badword with an empty word string!" << endl;
	}

	int wordContainer::findNextBoundry(int startPosition)
	{
		for (int i=startPosition; i<26; i++)
		{
			if (wordIndex[i] != -1)
				return i;
		}
		return wordList.size();
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

	void wordContainer::generateWordIndex()
	{
		//Improve, this won't work if not every character was a word eg no words that start with V
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
		wordIndexValid = true;
	}

	void wordContainer::printWordIndexBoundaries()
	{
		for (int i=0; i<25; i++)
		{
			cout << wordList[wordIndex[i]]->wordC << " ... " << wordList[wordIndex[i+1]-1]->wordC << endl;
		}
		cout << wordList[wordIndex[25]]->wordC << " ... " << wordList[wordList.size()-1]->wordC << endl;
	}

	void wordContainer::printWordIndexBoundariesSimple()
	{
		for (int i=0; i<26; i++)
		{
			cout << i << ": " << wordIndex[i] << endl;
		}
	}

	void wordContainer::removeWord(int wordPosition)
	{
		wordList.erase(wordList.begin()+wordPosition); //Remove the word* at position wordPosition. erase function takes a vector iterator which points at the word to be removed, begin function returns an iterator pointing to the start which is then incremented by wordPosition

		if (!wordIndexValid) //If the wordIndex is stale generate it again
			generateWordIndex();
		else //If the wordIndex was correct, lets update it rather than searching through all 20,000+ words again
		{
			for (int i=0; i<26; i++) //for all letter in the alphabet
			{
				if (wordIndex[i] > wordPosition) //if the position along the wordlist for a new starting character has passed the word we removed we want to shift all the values along by one
				{
					for (int j=i; j<26; j++)
					{
						wordIndex[j] = wordIndex[j] - 1;
					}
					break;
				}
				if (wordIndex[i] == wordPosition) //If they are equivilant then the new letter will still start at the same place but all later letters need to be shifted one
				{
					for (int j=i+1; j<26; j++)
					{
						wordIndex[j] = wordIndex[j] - 1;
					}
					break;
				}
			}
		}
	}

	void wordContainer::deleteWord(int wordPosition)
	{
		delete wordList[wordPosition];
		removeWord(wordPosition);
	}

	void wordContainer::addWord(word* wordToAdd)
	{
		//Needs to be improved/fixed
		wordList.push_back(wordToAdd);
	}

	void wordContainer::addBadWord(word* wordToAdd, string& badWordLine)
	{
		cout << wordToAdd << endl;
		cout << badWordLine << endl;
	}

	word* wordContainer::at(int wordToGet)
	{
		//This function gets the word from the wordPos list, eg sorted by wScore
		return wordList[wordPos[wordToGet]];
	}


	int wordContainer::findWordLocation(const string& wordToFind)
	{
		//Add exception for no word found
		//unsigned int wordListSize = wordList.size(); //minimise unnessissary function calls
		if (!wordIndexValid)
			generateWordIndex();
		int startPosition = wordIndex[static_cast<int>(wordToFind[0])-65]; //find the starting location to search for the word, wordToFind[0] is the first letter which is cast to an int between 65 and 90, removing 65 gives the letters position in the alphabet and so the position in the index array to get that letters starting position
		int stopPosition = wordIndex[static_cast<int>(wordToFind[0])-64];

		if (startPosition==-1)
		{
			return -1; //If the first letter has not been found, word does not exist
		}

		if (stopPosition==-1)
		{
			stopPosition=findNextBoundry(startPosition);

		}


		//cout << stopPosition-startPosition << endl;
		for (unsigned int i=startPosition; i< stopPosition; i++) //For every word with the same letter
		{
			if (wordToFind == wordList[i]->wordC) //if the word is found
				return i; //Return the position along the vector, also stops execution of loop
		}
		return -1; //If failed return an invalid position
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


	wordContainer::wordContainer()
	{
		cout << "Why is this constructor being called?" << endl;
		wordIndexValid = false;
	}

	/*
	wordContainer::wordContainer(wordContainer& fullWordList, string filename) //For use when building a bwordContainer from a file and the current known words
	{
		containsBadWords = true;
		vector<string> wrongWordVector = loadDictFile(filename);
		if (!fullWordList.wordIndexValid) //If the index is not valid, generate the index
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
	*/

	void wordContainer::wordWrong(int wordPosition, std::string attempt, wordContainer* containerToAddTo)
	{
		cout << wordList[wordPosition]->wordC << endl;
		string badWordLine = wordList[wordPosition]->wordWrong(attempt);
		cout << badWordLine << endl;
		badWord* badWordToAdd = new badWord(wordList[wordPosition],badWordLine);
		cout << badWordToAdd->wordFlags[0] << endl;
		delete badWordToAdd; //Remove when adding to a container
		if (containerToAddTo == this)
			cout << "You are trying to add to your self!" << endl;
		//Add overloaded functions that don't include which removing words or deleting words ect. Improve the add words, make indexing turn off able.
	}

	wordContainer::~wordContainer()
	{
		cout << "wordContainer object being destroyed, all its words will be deleted!" << endl;
		while (!wordList.empty())
		{
			delete wordList.back(); //returns the pointer at the back of the wordList and deletes the object it points to. As the destructor call is virtual the correct call will be made so no need to cast the pointer to the derived type
			wordList.pop_back(); //removes the last pointer from wordList which is destroyed
		}
	}

	badWord* wordContainer::getBadWord(int wordToGet)
	{
		if (wordToGet > -1)
		{
			if (wordToGet < wordList.size())
			{
				if (wordList[wordToGet]->wordFlags[0] == 1) //If the word is a bad word
					return static_cast<badWord*>(wordList[wordToGet]);
				else
					throw "wordContainer badWord* to word";
			}
			else
				throw "wordContainer Out of Bounds, int too large";
		}
		else
			throw "wordContainer Out of Bounds, negative!";

		//Add boundry checking and exception handling
		//Go back to fixing wordContainer::wordWrong
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
			cout << "Word: " << wordList[wordPos[i]]->wordC << " at " << wordPos[i] << " with a value of " << wordList[wordPos[i]]->wScore << endl;
		}
	}

	void wordContainer::DisplaywScores(int start, int stop)
	{
		if (stop ==-1)
			stop = wordList.size();
		for (unsigned int i=start; i<stop; i++)
		{
			cout << wordList[i]->wScore << ",";
		}
		cout << endl;
	}

	int wordContainer::size()
	{
		return wordList.size();
	}
