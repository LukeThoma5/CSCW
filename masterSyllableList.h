#ifndef masterSyllableList_h
#define masterSyllableList_h

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class masterSyllablesList
{
private:
	std::vector<int> syllablePos; //For mergesort

	void printVector(const std::vector<int>& sV);

	std::vector<int> splitVector(const std::vector<int> inVector, int mode);

	std::vector<int> syllableMergeSort(const std::vector<int>& inVector);

	void enactMergeSort(const std::vector<int>& inVector);

	std::unordered_map<std::string,int> syllableMap;

public:
	std::vector<std::string> syllables;
	std::vector<int> wrongCount;
	std::vector<float> weight;

	std::vector<std::string> findUniqueSyllables(const std::vector<std::string>& inSyllables);

	void addSyllables(const std::vector<std::string>& inSyllables);

	void addToTotal(const std::string& syllable, const int& amountToInc);

	void sortList();

	int size();

	bool hasNoValues();

	std::string operator[](int syllableToGet)
	{
		return syllables[syllableToGet];
	}

	int getSyllableWCount(int syllableToGet);

	int getSyllableWCount(const std::string syllable); //Overload to use map

};

#endif
