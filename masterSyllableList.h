#ifndef masterSyllableListTree_h
#define masterSyllableListTree_h

#include <vector>
#include <string>
#include <iostream>

#include "syllableNode.h"

class masterSyllablesList
{
private:
	// std::vector<int> syllablePos; //For mergesort

	// std::vector<int> splitVector(const std::vector<int> inVector, int mode);

	// std::vector<int> syllableMergeSort(const std::vector<int>& inVector);

	// void enactMergeSort(const std::vector<int>& inVector);

	// std::unordered_map<std::string,int> syllableMap;

	// std::vector<std::string> syllables;
	// std::vector<int> wrongCount;
	// std::vector<float> weight;

	bool isSortedOnWrongCount = false;
	syllableNode* root = nullptr;

	std::vector<syllableNode*> allNodes;

	std::vector<std::string> findUniqueSyllables(const std::vector<std::string>& inSyllables);


public:
	void makeAlphabetical();

	void addSyllables(const std::vector<std::string>& inSyllables);

	void addToTotal(const std::string& syllableToFind, const int& amountToInc);

	void sortList();

	int size();

	bool hasNoValues();

	std::string operator[](int syllableToGet)
	{
		return allNodes[syllableToGet]->getSyllableValue();
	}

	void print();

	int getSyllableWCount(int syllableToGet);

	int getSyllableWCount(const std::string syllableToFind); //Overload to use map

	~masterSyllablesList();

};

#endif