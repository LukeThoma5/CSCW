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

public:
    wordContainer goodWords;
    badwordContainer badWords;
    std::vector<int> wordPos;
    std::vector<bool> goodBadPos;
    wordCC();
    wordCC(std::string goodFilename, std::string badFilename);

    void generatewScore();

    void findHardest();

    word* getWord(int wordToGet);


    void printTop(int end=10);
};

#endif
