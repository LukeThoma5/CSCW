#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "randng.h" //My random number generator header
#include "masterSyllableList.h" //MSL declaration
#include "word.h"
#include "badWord.h"
#include "SSG.h"
#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"

using namespace std;

namespace SSG {
	masterSyllablesList MSL;
	wordContainer* goodWordList = NULL;
	wordContainer* badWordList = NULL;
}

void fillMasterSyllableList()
{
	for (unsigned int i=0; i<SSG::MSL.wrongCount.size(); i++)
	{
		SSG::MSL.wrongCount[i] = i*i;
	}
}

bool checkMasterSyllablesList()
{
	
}