#include "../../headers/wordCC.h"

using namespace std;

void printVector(const std::vector<int>& sV, int start=0, int stop=-1);
void printVector(const std::vector<bool>& sV, int start=0, int stop=-1);

void wordCC::generatewScore()
{
    cout << "Determining Scores" << endl;
	//Tell word containers to regenerate wordScores and sort themselves into decending order
    goodWords.sortWordContainer();
    badWords.sortWordContainer();
    badWords.DisplaywScores(); //Debug message, badwords usually smaller than goodwords.
}

void wordCC::findHardest()
//Function is always called after generatewScore
{
	//Empty out the goodBadPos and wordPos vectors ready for mergesort
    clearWordPos();
    combineWordLists(); //Combine the wordContainer abstractions to make a single interface
    currentWord=0; //Reset the current word to the begining of the wordCC
}

void wordCC::combineWordLists()
{
    //This assumes that generatewScore has already been called so the two lists are already sorted themselves.
	//Simple vector position pointers
	//Left is goodWords right is badwords
	int leftPos = 0;
    int rightPos = 0;
	//Cache the vector sizes as they won't change. Reduces function calls
    int leftVectorSize = goodWords.size();
    int rightVectorSize = badWords.size();

    while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
    {
        if (leftPos == leftVectorSize) //If out of left hand values add a right (badword)
        {
            wordPos.push_back(rightPos++); //add the right value to the return list and increment rightPos counter
            goodBadPos.push_back(true); //Mark the word as a badword
        }
        else
        {
            if (rightPos == rightVectorSize) //If out of right hand values add a left
            {
                //Add a left index then increment the local variable.
                wordPos.push_back(leftPos++);
                goodBadPos.push_back(false); //Mark the word as good
            }

            else
            {
                if (goodWords.at(leftPos)->getwScore() >= badWords.at(rightPos)->getwScore())
				//If the wrongCount that the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
                {
                    wordPos.push_back(leftPos++); //Add the leftvector position
                    goodBadPos.push_back(false); //Mark as a good word
                }
                else
				//The rightvector postition must point to a greater wrongCount the leftvector postition points to
                {
                    wordPos.push_back(rightPos++);
                    goodBadPos.push_back(true);
                }
            }
        }
    }
	if (wordPos.size() > 10 && goodBadPos.size() > 10) {
    printVector(wordPos,0,10);
    printVector(goodBadPos,0,10); }
}
