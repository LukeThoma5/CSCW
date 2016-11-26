#ifndef wordCC_h
#define wordCC_h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "wordContainer.h"
#include "badwordContainer.h"

class wordCC{

    void clearWordPos();

    void combineWordLists();

    void goodWordWrong(const int& wordPosition,const std::string& attempt);

    void fixwordPos(const int& goodWordLocation);

public:
    wordContainer goodWords;
    badwordContainer badWords;
    std::vector<int> wordPos;
    std::vector<bool> goodBadPos;
    wordCC();
    wordCC(std::string goodFilename, std::string badFilename);

    void generatewScore();

    void findHardest();

    word* getWord(const int& wordToGet);

    badWord* getBWord(const int& wordToGet);

    word* operator[](const int& wordToGet)
    {
        return getWord(wordToGet);
    }

    int findRealWordLocation(const std::string& comp);

    void wordWrong(const int& wordPosition,const std::string& attempt);

    void addWord(word* wordToAdd);


    void printTop(int start=0, int end=10);
};

#endif
