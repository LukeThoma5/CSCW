#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "word.h"
#include "badWord.h"
#include "wordContainer.h"

using namespace std;

//private

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

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void wordContainer::findWord(const string &s, string& badwordWord) {
	    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
	    getline(ss, badwordWord, '+');
	    	if (badwordWord.empty())
	        	cout << "Waring attempting to create badword with an empty word string!" << endl;
	}

//public

	void wordContainer::generateWordIndex()
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

	int wordContainer::findWordLocation(const string& wordToFind)
	{
		//Add exception for no word found
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
