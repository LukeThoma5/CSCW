#include <vector>
#include <string>
#include <iostream>

#include "../headers/masterSyllableListMap.h"

using namespace std;

//private

void printVector(const vector<string>& sV, int start=0, int stop=-1);
void printVector(const vector<int>& sV, int start=0, int stop=-1);
vector<int> splitVector(const vector<int>& inVector, int mode); //Now in main.cpp

void masterSyllablesListMap::addToTotal(const std::string& syllable, const int& amountToInc)
{
	std::unordered_map<std::string,int>::iterator iter = syllableMap.find(syllable); //Get a map iterator at the position of the syllable
	if (iter == syllableMap.end()) //If the syllable not in map
	{
		syllables.push_back(syllable); //Add the syllable with the wrongCount to increment
		wrongCount.push_back(amountToInc);
		weight.push_back(1.0);
		syllableMap[syllable] = syllables.size()-1; //Add the syllable to the map
	}
	else
	{
		wrongCount[iter->second] += amountToInc; //Increase the wrong count of the index held in the map value field (->second)
	}
}

vector<int> masterSyllablesListMap::syllableMergeSort(const vector<int>& inVector)
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

void masterSyllablesListMap::enactMergeSort(const vector<int>& inVector)
{
	syllablePos = syllableMergeSort(inVector); //Find the sorted syllables
	//Create the temps to store the values before commiting to objects vectors
	vector<string> syllablesTemp;
	vector<int> wrongCountTemp;
	vector<float> weightTemp;

	for (int i=0, syllablePosSize=syllablePos.size(); i<syllablePosSize; ++i)
	{
		syllablesTemp.push_back(syllables[syllablePos[i]]); //Add the syllables index stored at current index
		wrongCountTemp.push_back(wrongCount[syllablePos[i]]); //ADd the wrong count
		weightTemp.push_back(weight[syllablePos[i]]); //Add the weight
		syllableMap[syllables[syllablePos[i]]] = i; //Update the map to the correct index
	}
	//Commit the changes
	syllables = syllablesTemp;
	wrongCount = wrongCountTemp;
	weight = weightTemp;
}
//public
vector<string> masterSyllablesListMap::findUniqueSyllables(const vector<string>& inSyllables)
{
	if (syllables.size() == 0) //If no syllables all are unique
	{
		return inSyllables;
	}
	vector<string> returnVector; //Create a string vector
	bool unique = true; //Initialise value
	for (int i=0, inSyllSize=inSyllables.size(); i < inSyllSize; ++i)
	//For every syllable incoming
	{
		unique = true;
		for (unsigned int j=0; j<syllables.size(); j++)
		//For every syllable we currently have
		{
			if (inSyllables[i] == syllables[j]) //If we already have the syllable
			{
				unique = false; //Don't add the syllable
				break; //Stop searching this iteration
			}
		}
		if (unique) //If not found
		{
			returnVector.push_back(inSyllables[i]); //Add to list to add
		}
	}
	return returnVector; //Return the vector
}

void masterSyllablesListMap::addSyllables(const vector<string>& inSyllables)
{
	vector<string> syllablesToAdd = findUniqueSyllables(inSyllables); //Only add unique syllables
	for (int i=0, syllablesToAddSize=syllablesToAdd.size(); i < syllablesToAddSize; ++i)
	//For every unique syllable
	{
		syllables.push_back(syllablesToAdd[i]); //Add the syllable
		wrongCount.push_back(0); //It has been wrong 0 times, altered by badWord later
		weight.push_back(0.0); //Has no weight
		syllableMap[syllablesToAdd[i]] = syllables.size()-1; //Add the index to the unordered_map
	}
}

void masterSyllablesListMap::sortList()
{
	cout << "Sorting list" << endl;
	if (syllablePos.size() == 0) //If first run
	{
		for (int i=0; i<syllables.size(); i++) //Add a location for every syllable
		{
			syllablePos.push_back(i);
		}
	}
	else
	{
		for (int i=0; i<syllables.size(); i++) //Reset the locations to a simple order
		{
			syllablePos[i] = i;
		}
	}

	enactMergeSort(syllablePos); //Make the mergeSort
}

int masterSyllablesListMap::size()
{
	return syllables.size();
}

int masterSyllablesListMap::getSyllableWCount(int syllableToGet)
{
	return wrongCount[syllableToGet]; //Return the wrong count of the syllable at index
}

int masterSyllablesListMap::getSyllableWCount(const std::string syllable)
{
	return wrongCount[syllableMap[syllable]]; //Return wrongCount using the syllable and map
}

bool masterSyllablesListMap::hasNoValues()
{
	return true; //For testing purposes allways have random wrong createRandomWordWrongCounts

	for (int i=0; i<syllables.size(); i++) //For every syllable
	{
		if (wrongCount[i] != 0) //If a syllable has a value, exit
			return false;
	}
	return true; //All values must be 0, create random values
}

void masterSyllablesListMap::print()
{
	printVector(syllables);
	printVector(wrongCount);
}
//Need to define a destructor
masterSyllablesListMap::~masterSyllablesListMap() {cout<<"Destroying MSLMAP"<<endl;}
