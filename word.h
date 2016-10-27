#ifndef word_h
#define word_h

#include <string>
#include <vector>

class word
{
private:
	std::string generateBadWordLine(std::vector<int>& syllableWrongCount);

protected:
	//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
	void split(const std::string &s,const char delim, std::vector<std::string>& elems);


	std::vector<std::string> split(const std::string &s, const char delim='+');

	int determineSyllables();

	std::vector<int> determineWrongSyllables(const std::string& attempt);


public:
	std::string wordC;//wordCapitalised
	std::vector<std::string> syllables;
	std::vector<int> syllablePositions;
	std::string definition;
	std::vector<bool> wordFlags;
	//int size = 0;

	word(bool safteyMechanism);

	word(const std::string& wordline);

	virtual ~word();

	virtual std::string wordWrong(const std::string& attempt);

};

#endif
