#ifndef wordContainer_h
#define wordContainer_h

#include <vector>
#include <string>

#include "word.h"
#include "badWord.h"


class wordContainer {

	std::vector<std::string> loadDictFile(std::string filename="finalDictwithDef.txt");

	int wordIndex[26];
	bool containsBadWords = false;

	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void findWord(const std::string &s, std::string& badwordWord);

public:
	std::vector<word*> wordList;
	bool wordIndexValid = false;

	void generateWordIndex();

	void printWordIndexBoundaries();

	void removeWord(int wordPosition);

	void deleteWord(int wordPosition);

	void addWord(word* wordToAdd);

	void addBadWord(word* wordToAdd, std::string& badWordLine);

	void wordWrong(int wordPosition, std::string attempt, wordContainer* containerToAddTo);

	int findWordLocation(const std::string& wordToFind);

	wordContainer(std::string filename="finalDictwithDef.txt");

	wordContainer(wordContainer& fullWordList, std::string filename="wrongWords.txt"); //For use when building a bwordContainer from a file and the current known words

	~wordContainer();

};
#endif
