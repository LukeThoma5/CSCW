#include <iostream>
#include <sstream>
#include <fstream>

#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordContainer.h"

using namespace std;

vector<int> splitVector(const vector<int>& inVector, int mode); //Now in main.cpp

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

//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
void wordContainer::findWord(const string &s, string& badwordWord) {
    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
    getline(ss, badwordWord, '+');
    	if (badwordWord.empty())
        	cout << "Waring attempting to create badword with an empty word string!" << endl;
}

//public
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

wordContainer::~wordContainer()
{
	cout << "wordContainer object being destroyed, all its words will be deleted!" << endl;
	while (!wordList.empty()) //While not empty
	{
		delete wordList.back(); //returns the pointer at the back of the wordList and deletes the object it points to. As the destructor call is virtual the correct call will be made so no need to cast the pointer to the derived type
		wordList.pop_back(); //removes the last pointer from wordList which is destroyed
	}
}

bool wordContainer::wordCorrect(const int& correctWord)
{
	if (correctWord > size() || correctWord < 0)
		throw "wordContainer out of range";
	word* cWord = at(correctWord); //Get the word pointer out of the abstraction
	cout << cWord->getWord() << " is correct with a weight of " << cWord->getWeight() << " down to ";
	cWord->wordCorrect(); //Tell the word to reduce its weighting
	float wordWeight = cWord->getWeight(); //Get the new weighting
	cout << wordWeight << endl; //Finish the previous print statement

	if (wordWeight == 0.0)
		//If the item needs to be removed if a bad word, return true
		return true;
	return false;
}
