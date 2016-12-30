#ifndef masterSyllableListMap_h
#define masterSyllableListMap_h

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

#include "masterSyllableList.h"

class masterSyllablesListMap : public masterSyllablesList
{
private:
	std::vector<int> syllablePos; //For mergesort

	std::vector<int> syllableMergeSort(const std::vector<int>& inVector);

	void enactMergeSort(const std::vector<int>& inVector);

	std::unordered_map<std::string,int> syllableMap;

	std::vector<std::string> syllables;
	std::vector<int> wrongCount;
	std::vector<float> weight;

	std::vector<std::string> findUniqueSyllables(const std::vector<std::string>& inSyllables);

public:

	void addSyllables(const std::vector<std::string>& inSyllables);

	void addToTotal(const std::string& syllable, const int& amountToInc);

	void sortList();

	int size();

	bool hasNoValues();

	std::string operator[](int syllableToGet)
	{
		return syllables[syllableToGet];
	}

	void print();

	int getSyllableWCount(int syllableToGet);

	int getSyllableWCount(const std::string syllable); //Overload to use map

	~masterSyllablesListMap();

};

#endif
