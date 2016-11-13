#include <vector>
#include <string>
#include <iostream>

#include "masterSyllableList.h"

using namespace std;

//private

	void masterSyllablesList::printVector(const vector<int>& sV) //Remove
	{
		for (unsigned int i=0; i < sV.size(); i++)
		{
			cout << i << ":" << sV[i] << ' ';
		}
		cout << endl;
	}

	vector<int> masterSyllablesList::splitVector(const vector<int> inVector, int mode)
	{
		int start;
		int end;
		if (!mode) //if 0
		{
			start = 0;
			end = inVector.size()/2;
		}
		else
		{
			//if not 0
			start = inVector.size()/2;
			end = inVector.size();
		}
		vector<int> returnVector;
		for (int i=start; i<end; i++)
			returnVector.push_back(inVector[i]); //fill the vector with the values from original
		return returnVector;
	}

	vector<int> masterSyllablesList::syllableMergeSort(const vector<int>& inVector)
	{
		vector<int> leftVector = splitVector(inVector,0);  //Create left hand side with mode 0
		vector<int> rightVector = splitVector(inVector,1); //Create right hand side with mode 1

		int leftVectorSize = leftVector.size(); //Trying to minimise the amount of function calls
		int rightVectorSize = rightVector.size();

		if (leftVectorSize != 1) //if left needs to be sorted
			leftVector = syllableMergeSort(leftVector); //sort the left hand side
		if (rightVectorSize != 1)
			rightVector = syllableMergeSort(rightVector);

		//Now assumed the left and right vectors are sorted
		int leftPos = 0; //Simple vector position pointers
		int rightPos = 0;

		vector<int> returnVector;

		while ( (leftPos != leftVectorSize) or (rightPos != rightVectorSize) ) //If still more values to be added to the return vector
		{
			if (leftPos == leftVectorSize) //If out of left hand values add a right
			{
				returnVector.push_back(rightVector[rightPos++]); //add the next right value to the return list and increment rightPos counter
			}
			else
			{
				if (rightPos == rightVectorSize) //If out of right hand values add a left
				{
					returnVector.push_back(leftVector[leftPos++]);
				}

				else
				{
					if (wrongCount[leftVector[leftPos]] >= wrongCount[rightVector[rightPos]]) //If the wrongCount the leftvector postition points to is greater than the the wrongCount the rightvector postition points to
					{
						returnVector.push_back(leftVector[leftPos++]); //Add the leftvector position
					}
					else
					{
						returnVector.push_back(rightVector[rightPos++]);
					}
				}
			}
		}

		return returnVector;

	}

	void masterSyllablesList::enactMergeSort(const vector<int>& inVector)
	{
		syllablePos = syllableMergeSort(inVector);

		//printVector(syllablePos);
		//Set new positions

		/*
		cout << "Sorted list?" << endl;
		for (int j=0; j<syllablePos.size(); j++)
		{
			cout << syllablePos[j] << ": " << wrongCount[syllablePos[j]] << endl;
		}
		*/

		vector<string> syllablesTemp;
		vector<int> wrongCountTemp;
		vector<float> weightTemp;

		for (int i=0; i<syllablePos.size(); i++)
		{
			syllablesTemp.push_back(syllables[syllablePos[i]]);
			wrongCountTemp.push_back(wrongCount[syllablePos[i]]);
			weightTemp.push_back(weight[syllablePos[i]]);
		}

		syllables = syllablesTemp;
		wrongCount = wrongCountTemp;
		weight = weightTemp;
	}

//public

	vector<string> masterSyllablesList::findUniqueSyllables(const vector<string>& inSyllables)
	{
		if (syllables.size() == 0)
		{
			return inSyllables;
		}
		vector<string> returnVector;
		bool unique = true;
		for (unsigned int i=0; i < inSyllables.size(); i++)
		{
			unique = true;
			for (unsigned int j=0; j<syllables.size(); j++)
			{
				if (inSyllables[i] == syllables[j])
				{
					unique = false;
					break;
				}
			}
			if (unique)
			{
				returnVector.push_back(inSyllables[i]);
			}
		}
		return returnVector;
	}

	void masterSyllablesList::addSyllables(const vector<string>& inSyllables)
	{
		vector<string> syllablesToAdd = findUniqueSyllables(inSyllables);
		for (unsigned int i=0; i < syllablesToAdd.size(); i++)
		{
			syllables.push_back(syllablesToAdd[i]);
			wrongCount.push_back(0);
			weight.push_back(0.0);
		}
	}

	void masterSyllablesList::sortList()
	{
		cout << "Sorting list" << endl;
		if (syllablePos.size() == 0)
		{
			for (int i=0; i<syllables.size(); i++)
			{
				syllablePos.push_back(i);
			}
		}
		else
		{
			for (int i=0; i<syllables.size(); i++)
			{
				syllablePos[i] = i;
			}
		}

		enactMergeSort(syllablePos);
	}

	int masterSyllablesList::size()
	{
		return syllables.size();
	}

	int masterSyllablesList::getSyllableWCount(int syllableToGet)
	{
		return wrongCount[syllableToGet];
	}
