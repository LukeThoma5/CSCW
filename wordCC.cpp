#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"

using namespace std;

wordCC::wordCC()
{
    cout << "Default wordCC constructor called!" << endl;
}

wordCC::wordCC(std::string goodFilename, std::string badFilename) : goodWords(goodFilename), badWords(goodWords,badFilename)
{
        cout << "Creating wordContainers from filenames" << endl;
        goodWords.printWordIndexBoundariesSimple();
        badWords.printWordIndexBoundariesSimple();
        //Improve addWord, make it virtual and add a version for badwordContainer.
}

void wordCC::generatewScore()
{
    cout << "Determining Scores" << endl;

    //goodWords[8122]->determineScore();
    goodWords.sortWordContainer();
    badWords.sortWordContainer();

    goodWords.DisplaywScores(100,120);
    
    badWords.DisplaywScores();

}
