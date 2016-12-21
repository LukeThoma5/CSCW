#ifndef wordCC_h
#define wordCC_h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <ctime>

#include "wordContainer.h"
#include "badwordContainer.h"

class wordCC{

    void clearWordPos();

    void combineWordLists();

    void goodWordWrong(const int& wordPosition,const std::string& attempt);

    void fixwordPos(const int& goodWordLocation);

    void removeDuplicates(const std::vector<std::string>& dupWords);

    int notHave30goodWords();

    std::time_t keyboardStart = std::time(0);
    std::time_t spellingStart = std::time(0);
    int mistakes = 0;
    int keyboardWrongWordCount = 0;

public:
    wordContainer goodWords;
    badwordContainer badWords;
    std::vector<int> wordPos;
    std::vector<bool> goodBadPos;
    int currentWord = 0;

    wordCC();
    wordCC(std::string goodFilename, std::string badFilename);

    int size();

    void generatewScore();

    void findHardest();

    word* getWord(const int& wordToGet);

    word* getCurrentWord();

    int getCurrentPosition();

    void nextWord();

    badWord* getBWord(const int& wordToGet);

    word* operator[](const int& wordToGet)
    {
        return getWord(wordToGet);
    }

    int findRealWordLocation(const std::string& comp);

    void wordWrong(const int& wordPosition,const std::string& attempt);

    void wordCorrect(const int& wordPosition);

    void badwordCorrect(const int& wordPosition);

    void addWord(word* wordToAdd);

    void printwordCC(int stop=0);

    void spellingAttempt(const std::string& attempt);

    void findSpellingWords();

    void findKeyboardWords();

    bool keyboardAttempt(const std::string& attempt);

    void keyboardComplete();

    int keyboardCharCount();

    std::string getKeyboardWords();

    void printTop(int start=0, int end=10);
};

#endif
