#ifndef syllableNode_h
#define syllableNode_h

#include <string>
#include <iostream>
#include <vector>

class syllableNode
{
private:
	std::string syllable;
	int wrongCount;
	float weight;
	bool isOrderedByName = true;
	syllableNode* left = nullptr;
	syllableNode* right = nullptr;
public:

	//std::vector<std::string> findUniqueSyllables(const std::vector<std::string>& inSyllables);

	//void addSyllables(const std::vector<std::string>& inSyllables);

	void addToTotal(const int& amountToInc);

	syllableNode(const std::string& Syllable, const int WrongCount, const float Weight=1.0);

	//int size();

	bool hasNoValues();

	void clearPointers();

	void printInOrder();

	void addInOrder(std::vector<syllableNode*>& nodes);

	int getSyllableWCount() const;

	std::string getSyllableValue() const;

	void addValueOnSyllable(syllableNode* toAdd);

	void addValueOnWrongCount(syllableNode* toAdd);

	syllableNode* findNode(const std::string& toFind);

	~syllableNode();
};

#endif