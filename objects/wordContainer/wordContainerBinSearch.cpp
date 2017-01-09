#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <math.h>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"

using namespace std;

void printVector(const vector<string>& sV, int start=0, int stop=-1); //Early declaration
void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
vector<int> splitVector(const vector<int>& inVector, int mode); //Now in main.cpp

int wordContainer::stringCompare(const string& str1, const string& str2)
{

    if (str1 == str2)
        return 0;

    //Strings must now be different
    int str1size = str1.size(); //Force the string size to a signed int
    int str2size = str2.size();
    int stop = str1size; //Initialise to first string end

    int retInt = -1; //Initialise to first string

    if (str1size != str2size) //If sizes don't match
    {
        if (str1size > str2size) //If the second string is smaller, set the end to the smallest string
        {
            stop = str2size;
            retInt = 1; //Set the return value to the second string
        }
        //If 2size > 1size not nessissary as values initialised in that form
    }

    auto s1pos = str1.begin(); //Create an iterator at the start of each string
    auto s2pos = str2.begin();

    auto s1end = str1.end(); //Cache iterator end
    auto s2end = str2.end();

    while (s1pos <  s1end && s2pos < s2end) //While still got letters to compare
    {

        if (*s1pos == *s2pos) //Dereference the iterators, if they are the same increment both
        {
            ++s1pos; //Pre increment to reduce compiler temporaries
            ++s2pos;
            continue;
        }

        if (static_cast<int>(*s1pos) < 65) //If the char value is less than the ASCII letter 'A' eg not A-Z
        {
            ++s1pos;
            continue;
        }

        if (static_cast<int>(*s2pos) < 65)
        {
            ++s2pos;
            continue;
        }

        if (*s1pos > *s2pos) //If they are not the same or an invalid character, return based on the smaller char
            return 1;
        return -1;
    }
    return retInt;
}

int wordContainer::binSearch(const std::string& comp, int start, int stop)
{

    //string pause;
    //cin >> pause;
    //cout << "----------------------------------------------------------------------\n";
    //cout << "Start point " << start << " stop point " << stop << endl;
    float midFloat =start+stop;
    midFloat = midFloat / 2;
    int midpoint = ceil(midFloat);
    //cout << "Midpoint is " << midpoint << " " << midFloat << endl;
    //cout << "Checking " << wordList[midpoint]->getWord() << endl;
    if (start == stop) //Bottom out recursion, if can't split again
    {
        if (comp == wordList[start]->getWord()) //If the word we are looking for, return the position
            return start;
        return -1; //If not found, return invalid position
    }

    if (stop-start == 1) //If on last 2 words, check each individually
    {
        //int compres = stringCompare(comp,wordList[start]->getWord();
        //cout << "Checking " << wordList[start]->getWord() << endl;
        if (comp ==  wordList[start]->getWord())
            return start;
        //cout << "Checking " << wordList[stop]->getWord() << endl;
        if (comp == wordList[stop]->getWord())
            return stop;
        return -1;
    }

    int comparisonResult = stringCompare(comp,wordList[midpoint]->getWord());
    //comp.compare(wordList[midpoint]->getWord()); doesn't work as it handles special characters differently to the dictionary

    //cout << "Expecting " << comp.compare(wordList[midpoint]->getWord()) << " got " << comparisonResult << endl;

    if (comparisonResult == 0) //If the word, return position
        return midpoint;
    if (comparisonResult > 0) //If later, return the value of a searchin the later half
    {
        //if (stop-midpoint == 1)
            //return binSearch(comp,midpoint,midpoint);
        //cout << "searching at " << midpoint << " " << stop << endl;
        return binSearch(comp,midpoint,stop);
    }
    if (comparisonResult < 0) //If earlier, return the value of a searchin the earlier half
    {
        //if (midpoint-start == 1)
            //return binSearch(comp,start,start);
        //cout << "searching at " << start << " " << midpoint << endl;
        return binSearch(comp,start,midpoint);
    }

    cout << "An error has occured in searching for word " << comp << endl;
    throw -25;
}

int wordContainer::findWordLocation(const string& wordToFind)
{
    return binSearch(wordToFind,0,wordList.size());
}
