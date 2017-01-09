#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "../../headers/word.h"
#include "../../headers/hLog.h"
#include "../../headers/masterSyllableList.h"

#include "math.h"
#include <complex>

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
	extern hLog histLog;
}

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);

string word::generateBadWordLine(vector<int>& syllableWrongCount)
{
    string badWordLine = wordC + "+1.0";
    for (int i =0; i<syllableWrongCount.size(); i++)
    {
        badWordLine += ("+" + to_string(syllableWrongCount[i]));
    }
    return badWordLine;
}

vector<int> word::determineWrongSyllables(const string& attempt)
{
    vector<int> syllableWrongCount; //Create a vector to store the values for the badWord syllableWrongCount
    syllableWrongCount.reserve(syllables.size()); //To prevent unnessissary memory management make sure that the vector is atleast as long as the amount of syllables
    int attemptEnd = wordC.size();
    if (attempt.size() != wordC.size()) //If they are not the same length determine how far is safe to check so the program doesn't crash
    {
        if (attempt.size() < wordC.size())
            attemptEnd = attempt.size();
        //Reverse not nessissary as attemptEnd is initalised to wordC.size()
    }

    int syllableEndPos;
    bool isSyllableWrong = false;
    for (int i=0; i<syllables.size(); i++)
    {
        //cout << "Doing Checks for Syllable: " << i << endl;
        if (i != (syllables.size()-1))
            syllableEndPos = syllablePositions[i+1]; //The place to stop checking the current syllable is the position of the start of the next
        else
        {
            //cout << "syllables size: "  << syllables.size() << endl;
            //cout << "Attempt size: " << attempt.size() << endl;
            //cout << "WordC size: " << wordC.size() << endl;
            syllableEndPos = wordC.size(); //If on the last syllable, the place to stop checking at is the end of the word
            //cout << "syllableEndPos: " << syllableEndPos << endl;
        }
        isSyllableWrong = false; //Has an incorrect syllable been found in the current syllable
        for (int j=syllablePositions[i]; j<syllableEndPos; j++)
        {
            //cout << "Checking syllable: " << i << endl;
            if (syllableEndPos > attemptEnd) //If the syllable will overrun, the syllable must be wrong
            {
                isSyllableWrong = true;
                break;
            }
            if (attempt[j] != wordC[j]) //If the attempt has a wrong letter within the current syllable break
            {
                isSyllableWrong = true;
                break;
            }
        }
        if (isSyllableWrong) //If a wrong letter has been found mark the syllable as wrong
        {
            syllableWrongCount.push_back(1);
        }
        else
        {
            syllableWrongCount.push_back(0);
        }
    }

    if (attempt.size() > wordC.size())
    {
        syllableWrongCount[syllableWrongCount.size()-1] = 1;
    }

    return syllableWrongCount;
}

string word::wordWrong(const string& attempt)
{

    cout << "Original wrongWord called!" << endl;
    vector<int> syllableWrongCount = determineWrongSyllables(attempt); //Create a vector to store the values for the badWord syllableWrongCount


    printVector(syllables);
    printVector(syllableWrongCount);
    printVector(syllablePositions);

    string badWordLine = generateBadWordLine(syllableWrongCount);

    cout << badWordLine << endl;

    //addToMSLTotal(syllableWrongCount);

    return badWordLine;

    //badWord mybadWord("ADEQUATELY+AE1+D+AH0+K+W+IH0+T+L+IY0+#DEF+In an adequate manner.","ADEQUATELY+1.0+0+0+0+0+0+0+0+0+0");

    //badWord* badWordToAdd = new badWord(this,badWordLine);

    //badWordContainer->addBadWord(this,badWordLine);

}