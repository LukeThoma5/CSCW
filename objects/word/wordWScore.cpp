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

void word::wScoreHelper()
{
    wScore = 0.0; //Reset wScore to start from a clean slate
    for (unsigned int i=0; i<syllables.size(); i++) //For every syllable this word has
    {
		//Gets the wrongCount for the string passed, using an unordered_map (If MSLMap)
		//Use MSL interface to extract the wrong count of the current syllable
        wScore += SSG::MSL->getSyllableWCount(syllables[i]);
    }

    const int offset = 15; //Use this to alter the affect that wordSize has on end wScore, a higher value makes word length matter less
    const int ideal = 7; //The target word size
    const int denom = ideal + offset; //A compile time constant to reduce the amount of math required at runtime
    signed int wordSize = wordC.size(); //Forcing the size of the word from an unsigned int to a signed int so it wont underflow

    #ifdef WORDDEBUG
    //Error checking and Algorithm testing code
    if ((denom-abs(wordSize-ideal))/(denom) > 1.0)
    {
        cout << "wScore for " << wordC << " is " << wScore << endl;
        cout << "The penalty is " << (abs(wordSize-ideal)+offset)/(denom) << endl;
        cout << "The absolute value is " << abs(wordSize-ideal) << " the value was " << wordSize-ideal << endl;
        cout << "The final score is: " << (wScore/syllables.size()) * (denom-abs(wordSize-ideal))/(denom) << endl;
    }

    double numerator = (denom-abs(wordSize-ideal));
    double penalty = numerator/denom;

    float rwScore = (wScore*penalty)/syllables.size();
    float twScore = (wScore*(denom-abs(wordSize-ideal)))/(denom*syllables.size());

    if (rwScore != twScore)
    {
        cout << "An error has occured " << rwScore << " != " << twScore << endl;
        cout << rwScore << " " << twScore << " " << rwScore-twScore << endl;
    }

    float wTotal = wScore;
    wScore = rwScore;

    if (wScore == 0.0)
    {
        float numerator = (denom-abs(wordSize-ideal));
        float penalty = numerator/denom;
        cout << "ZERO wScore!!!" << endl;
        cout << "wScore for " << wordC << " is " << wTotal << endl;
        cout << "The penalty is " << ((denom-abs(wordSize-ideal))/(denom)) << " which should be " << penalty << endl;
        cout << "The numerator is " << numerator << endl;
        cout << "The denomenator is " << denom << endl;
        cout << "The absolute value is " << abs(wordSize-ideal) << " the value was " << wordSize-ideal << endl;
        cout << "The final score is: " << ((wScore/syllables.size()) * ((denom-abs(wordSize-ideal))/(denom))) << endl;
    }
    #endif

    /*
    Documentation
    Average syllable contribution = [Sum of syllable Wrong Count for this word] / [# Number of syllables for this word]
    is multiplied by a penalty <= 1.0 which is there to reduce the effect of very long words.
    penalty = [offset+ideal-magnitude(length-ideal)] / [offset+ideal]
    magnitude(length-ideal) gives you the number of letters away from the ideal number.
    denom = k = offset+ideal

    wScore = [Average syllable contribution] * penalty

    In its 'simplest' form it is:

    numerator = [[Sum of syllable Wrong Count for this word] * (K-magnitude(length-ideal))]
    denomenator = [K(# Number of syllables for this word)]

    wScore = numerator / denomenator

    To decrease the effect that being near the ideal size has, increase the offset.
    */
    wScore = (wScore*(denom-abs(wordSize-ideal)))/(denom*syllables.size());
}

void word::determineScore()
{
    wScoreHelper();
    wScore *= weight; //Makes good words less valuable than badWords
}
