#ifndef SSG_h
#define SSG_h

class wordContainer; //Early decleration

#include "masterSyllableList.h"
#include "wordContainer.h"

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	extern masterSyllablesList MSL; //MasterSyllablesList
	extern wordContainer* goodWordList; //Allows the currently worked on wordList to be updated and interchanged
	extern wordContainer* badWordList; //Defaulting to null prevents manipulating objects that don't exist.
}


#endif