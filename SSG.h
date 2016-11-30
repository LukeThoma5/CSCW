#ifndef SSG_h
#define SSG_h

class wordContainer; //Early decleration

#include "masterSyllableList.h"
#include "wordContainer.h"
#include <gtkmm.h>
#include "windowContainer.cpp"
#include "wordCC.h"

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	extern masterSyllablesList MSL; //MasterSyllablesList
	extern wordContainer* goodWordList; //Allows the currently worked on wordList to be updated and interchanged
	extern wordContainer* badWordList; //Defaulting to null prevents manipulating objects that don't exist.
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
}


#endif