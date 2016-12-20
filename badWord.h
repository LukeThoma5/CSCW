#ifndef badWord_h
#define badWord_h

#include <string>
#include <vector>

#include "word.h"

class badWord : public word { //New class badWord inherits from word, keep datamembers public

protected:
	void constructorHelper(const std::string& badwordline);

public:

	std::vector<int> syllableWrongCount;
	//float weight;

	badWord(const std::string& wordline, const std::string& badwordline);

	badWord(const badWord& createFrom);

	badWord(const word* createFrom, std::string badWordLine);

	std::vector<int> getSyllableWrongCount();

	virtual std::string wordWrong(const std::string& attempt);

	virtual void determineScore();

	std::string getBadWordLine();
};

#endif
