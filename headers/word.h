#ifndef word_h
#define word_h

#include <string>
#include <vector>

void split(const std::string &s,const char delim, std::vector<std::string>& elems);
std::vector<std::string> split(const std::string &s, const char delim='+');

class word
{
protected:
	std::string generateBadWordLine(const std::vector<int>& syllableWrongCount);

	int determineSyllables();

	std::vector<int> determineWrongSyllables(const std::string& attempt);

	void wScoreHelper();

	void addToMSLTotal(const std::vector<int>& syllableWrongCount) const;

	std::string wordC;//wordCapitalised
	std::vector<std::string> syllables;
	std::vector<int> syllablePositions;
	std::string definition;
	std::vector<bool> wordFlags;
	float wScore = 0.0;
	float weight = 0.5;
public:
	//int size = 0;

	word(bool safteyMechanism);

	word(const std::string& wordline);

	word(word* bwordToSlice);

	std::string getWord() const;

	std::vector<std::string> getSyllables() const;

	std::vector<int> getSyllablePositions() const;

	std::string getDefinition() const;

	std::vector<bool> getWordFlags() const;

	float getwScore() const;

	float getWeight() const;

	virtual void determineScore();

	virtual bool wordCorrect();

	virtual ~word();

	virtual std::string wordWrong(const std::string& attempt);

};

#endif
