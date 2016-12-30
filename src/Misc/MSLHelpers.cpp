#include <iostream>

#include "../../headers/randng.h" //random number generator header
#include "../../headers/masterSyllableList.h"

namespace SSG {
	extern masterSyllablesList* MSL; //MasterSyllablesList
}

void createRandomWordWrongCounts() //Fill the MSL with random wrongCounts to test sorting
{
	const int HIGH = 12; //Was 450
	const int LOW = 0;

	for (unsigned int i=0; i<SSG::MSL->size(); i++)
	{
		SSG::MSL->addToTotal((*SSG::MSL)[i],randNG(LOW,HIGH)); //Use just the interface //Valgrind does not like this call
		//SSG::MSL->addToTotal((*SSG::MSL)[i],0); //This can be used to memory leak testing
	}
}

masterSyllablesList::~masterSyllablesList() {std::cout<<"ClosingMSL"<<std::endl;}
