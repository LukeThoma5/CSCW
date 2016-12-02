#ifndef wordContainer_h
#define wordContainer_h

#include <vector>
#include <string>

#include "word.h"
#include "badWord.h"


class wordContainer {
protected:
	std::vector<std::string> loadDictFile(std::string filename="finalDictwithDef.txt");

	bool containsBadWords = false;

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void findWord(const std::string &s, std::string& badwordWord);

	void refreshwScores();

	void fillWordPos();

	std::vector<int> wordPos;

	std::vector<int> splitVector(const std::vector<int>& inVector, int mode);

	std::vector<int> containerMergeSort(const std::vector<int>& inVector);

	int findWordInsertionPoint(const std::string& searchWord);

public:
	std::vector<word*> wordList;

	void removeWord(int wordPosition);

	void deleteWord(int wordPosition);

	int addWord(word* wordToAdd);

	void addBadWord(word* wordToAdd, std::string& badWordLine);

	void wordWrong(int wordPosition, std::string attempt, wordContainer* containerToAddTo);

	int findWordLocation(const std::string& wordToFind);

	void sortWordContainer();

	int size();

	int binSearch(const std::string& comp, int start, int stop);

	int stringCompare(const std::string& str1, const std::string& str2);

	word* operator[](int wordToGet)
	{
		//a[b] opperator cannot be a non-member function
		if (wordToGet > -1)
		{
			if (wordToGet < wordList.size())
			{
				return wordList[wordToGet];
			}
			else
				throw "wordContainer Out of Bounds, int too large";
		}
		else
			throw "wordContainer Out of Bounds, negative!";
	}

	word* at(int wordToGet);

	badWord* getBadWord(int wordToGet);

	void DisplaywScores(int start=0, int stop=-1);

	void printWordContainer();

	int getABSIndex(const int& wordToGet);

	wordContainer();

	wordContainer(std::string filename); //Can't default the value or else will get ambiguity error as 2 constructors callable with no arguments

	~wordContainer();

};
#endif
