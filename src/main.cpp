#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include <gtkmm.h>

#include "../headers/randng.h" //random number generator header
#include "../headers/masterSyllableList.h" //MSL declaration
#include "../headers/masterSyllableListTree.h"
#include "../headers/masterSyllableListMap.h"
#include "../headers/wordCC.h"
#include "../headers/hLog.h"

#include "../objects/windowContainer.cpp"

using namespace std;

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	masterSyllablesListTree physicalMSL; //Define the MSLTree object that will fufill the MSL interface
	//masterSyllablesListMap physicalMSL;
	masterSyllablesList* MSL = &physicalMSL; //MasterSyllablesList //Upcast either MSLTree or MSLMap to MSL so objects can interact with it.
	Glib::RefPtr<Gtk::Builder> refBuilder; //Gtk object needed to retrieve widgets from the gtk builder (object that loads the xml file)
	windowContainer winContainer; //Object to keep the Gtk::window pointers active
	//wordCC SpellingWords("./Data/finalDictwithDef.txt", "./Data/wrongWords.txt");
	wordCC SpellingWords("./Data/shortDict.txt", "./Data/shortwrongWords.txt"); //Load the words into memory and store in a wordCC (wordContainerContainer)
	hLog histLog("./Data/SSGHistFile.txt"); //Load the historyLog from a file
	extern const time_t sessionStartTime = time(0); //Set a constant that has external linkage (can be seen by other translation units) to the time at startup of the program
}

void createRandomWordWrongCounts();
void connectSignals();

int main (int argc, char **argv)
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    //Load the GtkBuilder file and instantiate its widgets:
    SSG::refBuilder = Gtk::Builder::create();
    try
    {
      SSG::refBuilder->add_from_file("SSG_Gui_rev2.glade");
    }
    catch(const Glib::FileError& ex)
    {
      std::cerr << "FileError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
      std::cerr << "MarkupError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
      std::cerr << "BuilderError: " << ex.what() << std::endl;
      return 1;
    }

	SSG::winContainer.addWindows(SSG::refBuilder);
	connectSignals();

	if (SSG::MSL->hasNoValues())
		createRandomWordWrongCounts();

	app->run(*(SSG::winContainer.MainScreen));

	cout << "Returning to OS" << endl;

	return 0;
}
