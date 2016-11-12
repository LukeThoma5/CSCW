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
public:
    wordContainer goodWords;
    badwordContainer badWords;
    wordCC();
    wordCC(std::string goodFilename, std::string badFilename);
};

#endif
