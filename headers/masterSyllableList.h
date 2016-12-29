#ifndef masterSyllableList_h
#define masterSyllableList_h

#include <vector>
#include <string>
#include <iostream>

class masterSyllablesList
{
public:
	virtual void addSyllables(const std::vector<std::string>& inSyllables) = 0;

	virtual void addToTotal(const std::string& syllableToFind, const int& amountToInc) = 0;

	virtual void sortList() = 0;

	virtual int size() = 0;

	virtual bool hasNoValues() = 0;

	virtual std::string operator[](int syllableToGet) = 0;

	virtual void print() = 0;

	virtual int getSyllableWCount(int syllableToGet) = 0;

	virtual int getSyllableWCount(const std::string syllableToFind) = 0; //Overload to use map

	virtual ~masterSyllablesList() = 0;
};

#endif
