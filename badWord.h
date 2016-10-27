#ifndef badWord_h
#define badWord_h

#include <string>
#include <vector>

#include "word.h"

void printVector(const std::vector<std::string>& sV); //Early declaration
void printVector(const std::vector<int>& sV);

class badWord : public word { //New class badWord inherits from word, keep datamembers public

protected:
	void constructorHelper(const std::string& badwordline);

public:

	std::vector<int> syllableWrongCount;
	float weight;

	badWord(const std::string& wordline, const std::string& badwordline);

	badWord(const badWord& createFrom);

	badWord(const word* createFrom, std::string badWordLine);

	virtual std::string wordWrong(const std::string& attempt);
};

#endif
